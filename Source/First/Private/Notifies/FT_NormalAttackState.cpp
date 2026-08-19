#include "Notifies/FT_NormalAttackState.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "KismetTraceUtils.h"
#include "GamePlayTags/FTTag.h"

void UFT_NormalAttackState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	// 本次攻击窗口开始：清空命中记录 → 每招对每个对象只命中一次
	HitActorsByMesh.FindOrAdd(MeshComp).Reset();
	// 首帧没有"上一位置"，用当前 Socket 初始化 → 第一帧退化为点扫，之后开始运动轨迹扫描
	PrevSocketByMesh.FindOrAdd(MeshComp) = MeshComp->GetSocketTransform(SocketName).GetLocation();
}

void UFT_NormalAttackState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	const auto HitResults = PerformSphereTrace(MeshComp);
	SendEventToActors(HitResults, MeshComp);
}

void UFT_NormalAttackState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	// 窗口结束：清理该角色的命中记录与上一帧位置
	HitActorsByMesh.Remove(MeshComp);
	PrevSocketByMesh.Remove(MeshComp);
}

TArray<FHitResult> UFT_NormalAttackState::PerformSphereTrace(USkeletalMeshComponent* MeshComp)
{
	const FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);
	const FVector CurrentSocket = SocketTransform.GetLocation();
	const FVector PrevSocket = PrevSocketByMesh.FindOrAdd(MeshComp);

	// 运动轨迹扫描：上一帧 Socket 位置 → 当前帧位置，高速挥砍时武器路径上的对象不漏扫（防穿透）
	const FVector Delta = CurrentSocket - PrevSocket;
	const float DeltaLen = Delta.Size();
	FVector Start;
	FVector End;
	if (DeltaLen > KINDA_SMALL_NUMBER)
	{
		const FVector Dir = Delta / DeltaLen;
		Start = PrevSocket - Dir * SocketExtensionOffset;   // 沿运动方向两端各留一段缓冲
		End = CurrentSocket + Dir * SocketExtensionOffset;
	}
	else
	{
		// 原地未动（含首帧）：退回原"沿武器方向"的点扫
		Start = CurrentSocket;
		End = CurrentSocket - SocketTransform.GetRotation().GetForwardVector() * SocketExtensionOffset;
	}
	PrevSocketByMesh.FindOrAdd(MeshComp) = CurrentSocket;

	TArray<FHitResult> OutHits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MeshComp->GetOwner());

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);

	UWorld* World = GEngine->GetWorldFromContextObject(MeshComp, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
		return OutHits;
	const bool bHit = World->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_Visibility,
	                                             FCollisionShape::MakeSphere(SphereTraceRadius), Params,
	                                             ResponseParams);

	if (bDrawDebug)
		DrawDebugSphereTraceMulti(World,
		                          Start,
		                          End,
		                          SphereTraceRadius,
		                          EDrawDebugTrace::ForDuration,
		                          bHit,
		                          OutHits,
		                          FColor::Red,
		                          FColor::Green,
		                          5.f);

	// 本次 notify 窗口内已命中的对象不重复命中：仅返回并记录新增命中的对象
	TArray<TWeakObjectPtr<AActor>>& AlreadyHit = HitActorsByMesh.FindOrAdd(MeshComp);
	OutHits.RemoveAll([&AlreadyHit](const FHitResult& Hit)
	{
		AActor* Actor = Hit.GetActor();
		return !Actor || AlreadyHit.Contains(TWeakObjectPtr(Actor));
	});
	for (const FHitResult& Hit : OutHits)
	{
		if (AActor* Actor = Hit.GetActor())
		{
			AlreadyHit.Add(TWeakObjectPtr(Actor));
		}
	}

		return OutHits;
}

void UFT_NormalAttackState::SendEventToActors(TArray<FHitResult> Hits, const USkeletalMeshComponent* MeshComp)
{
	if (!IsValid(MeshComp))
		return;

	AActor* Attacker = MeshComp->GetOwner();
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor || HitActor == Attacker)
			continue;

		UAbilitySystemComponent* AbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (!IsValid(AbilitySystem) || AbilitySystem->HasMatchingGameplayTag(FTTag::Status::Dead))
			continue;

		FGameplayEffectContextHandle EffectContext = AbilitySystem->MakeEffectContext();
		EffectContext.AddHitResult(Hit);

		FGameplayEventData PayloadData;
		PayloadData.Target = HitActor;
		PayloadData.ContextHandle = EffectContext;
		PayloadData.Instigator = Attacker;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Attacker, FTTag::Events::AttackHit, PayloadData);
	}
}

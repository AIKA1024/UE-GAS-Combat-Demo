#include "AbilitySystem/Abilities/FT_HitReactAbility.h"

#include "AbilitySystem/Combat/UFT_HitReactFunctionLibrary.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

UFT_HitReactAbility::UFT_HitReactAbility()
{
	// 需要每实例状态（当前蒙太奇），必须 InstancedPerActor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 播放中再次命中 → 引擎结束当前激活并重新激活（ActivateAbility 重播蒙太奇）。
	// 相比自监听命中事件重播，没有"回声"问题，也无需手动管理监听句柄
	bRetriggerInstancedAbility = true;
}

void UFT_HitReactAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAnimInstance* Anim = nullptr;
	UAnimMontage* MontageToPlay = nullptr;
	if (ActorInfo && ActorInfo->AvatarActor.IsValid())
	{
		if (USkeletalMeshComponent* Mesh = ActorInfo->AvatarActor->FindComponentByClass<USkeletalMeshComponent>())
			Anim = Mesh->GetAnimInstance();
		MontageToPlay = GetReactMontage(ActorInfo->AvatarActor.Get());
	}

	if (Anim && MontageToPlay)
	{
		PlayReactMontage(Anim, MontageToPlay, TriggerEventData);
	}
	else
	{
		// 没取到蒙太奇（角色没配/子类没回退）：仅执行打断/封锁逻辑后立即结束
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UFT_HitReactAbility::PlayReactMontage(UAnimInstance* Anim, UAnimMontage* MontageToPlay, const FGameplayEventData* Payload)
{
	// 递增令牌：本次激活绑定的蒙太奇实例回调才被认可。
	// 重触发时旧实例被打断 blend-out 后其结束回调会迟到，且回调只传蒙太奇资产指针
	// （重触发前后是同一资产，无法靠指针甄别）→ 用令牌挡掉旧实例的迟到回调
	const uint64 Token = ++ReactMontageToken;

	// 用按次调用的 BlendIn 参数做淡入（不修改蒙太奇资产）：
	// 重触发时新实例从权重 0 按 HitReactBlendTime 淡入，
	// 旧实例被同组规则以相同混合设置淡出，形成交叉过渡
	Anim->Montage_PlayWithBlendIn(MontageToPlay, FAlphaBlendArgs(HitReactBlendTime), 1.f);

	if (Payload && IsValid(Payload->Instigator) && IsValid(Payload->Target))
	{
		const FVector Dir = UFT_HitReactFunctionLibrary::ComputeHitDirection(
			Payload->Instigator->GetActorLocation(),
			Payload->Target->GetActorLocation(),
			Payload->Target->GetActorRotation());
		const FName SectionName = UFT_HitReactFunctionLibrary::GetHitReactSectionNameByFVector(Dir);
		Anim->Montage_JumpToSection(SectionName);
	}

	// 绑定该蒙太奇的结束回调：播放完/被打断都会触发 → 结束能力
	FOnMontageEnded EndedDelegate;
	EndedDelegate.BindWeakLambda(this, [this, Token](UAnimMontage* Montage, bool bInterrupted)
	{
		// 旧实例的迟到回调令牌已过期，直接忽略（否则会提前结束能力 → 硬直期间恢复移动）
		if (Token != ReactMontageToken)
			return;

		OnReactMontageEnded(Montage, bInterrupted);
	});
	Anim->Montage_SetEndDelegate(EndedDelegate, MontageToPlay);
}

void UFT_HitReactAbility::OnReactMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo)
		return;

	EndAbility(GetCurrentAbilitySpecHandle(), ActorInfo, GetCurrentActivationInfo(), true, bInterrupted);
}

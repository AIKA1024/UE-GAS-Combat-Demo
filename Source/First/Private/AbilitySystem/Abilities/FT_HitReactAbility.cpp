#include "AbilitySystem/Abilities/FT_HitReactAbility.h"

#include "AbilitySystem/Combat/UFT_HitReactFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

UFT_HitReactAbility::UFT_HitReactAbility()
{
	// 需要每实例状态（蒙太奇结束委托句柄），必须 InstancedPerActor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFT_HitReactAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 先播放蒙太奇，再注册事件监听
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
		return;
	}

	// 播放中再次命中 → 重播受击动画：监听本能力的事件触发器 tag
	//（能力激活态下 GAS 的 Spec->IsActive() 会拒绝再次激活，必须自己在播放期间监听命中）
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		const FGameplayTagContainer TriggerTags = CollectTriggerEventTags();
		if (!TriggerTags.IsEmpty())
		{
			HitEventHandle = ASC->AddGameplayEventTagContainerDelegate(
				TriggerTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnHitEventWhileActive));
		}
	}
}

void UFT_HitReactAbility::PlayReactMontage(UAnimInstance* Anim, UAnimMontage* MontageToPlay, const FGameplayEventData* Payload)
{
	CurrentReactMontage = MontageToPlay;
	// 用按次调用的 BlendIn 参数做淡入（不修改蒙太奇资产）：
	// 重播时引擎会新建蒙太奇实例，从权重 0 按 HitReactBlendTime 淡入，
	// 与旧实例（OnHitEventWhileActive 里 Montage_Stop）的淡出形成交叉过渡
	Anim->Montage_PlayWithBlendIn(MontageToPlay, FAlphaBlendArgs(HitReactBlendTime), 1.f);

	if (Payload && IsValid(Payload->Instigator) && IsValid(Payload->Target))
	{
		const FVector Dir = UFT_HitReactFunctionLibrary::ComputeHitDirection(
			Payload->Instigator->GetActorLocation(),
			Payload->Target->GetActorLocation(),
			Payload->Target->GetActorRotation());
		const FName SectionName = UFT_HitReactFunctionLibrary::GetHitReactSectionNameByFVector(Dir);
		Anim->Montage_JumpToSection(SectionName);
		
		// 调试：显示调用堆栈
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, 
			FString::Printf(TEXT("PlayReactMontage: %s, Time: %.3f"), 
				*SectionName.ToString(), 
				GetWorld() ? GetWorld()->GetTimeSeconds() : -1.f));
	}

	// 绑定该蒙太奇的结束回调：播放完/被打断都会触发 → 结束能力
	FOnMontageEnded EndedDelegate;
	EndedDelegate.BindUObject(this, &ThisClass::OnReactMontageEnded);
	Anim->Montage_SetEndDelegate(EndedDelegate, MontageToPlay);
}

void UFT_HitReactAbility::OnHitEventWhileActive(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	// 重复事件已在攻击方按目标去重（UFT_AttackGamePlayAbility::SettledTargets），
	// 这里收到的每个 Hit.* 事件都对应一次真实命中，直接重播受击动画
	UAnimInstance* Anim = nullptr;
	UAnimMontage* MontageToPlay = nullptr;
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (ActorInfo && ActorInfo->AvatarActor.IsValid())
	{
		if (USkeletalMeshComponent* Mesh = ActorInfo->AvatarActor->FindComponentByClass<USkeletalMeshComponent>())
			Anim = Mesh->GetAnimInstance();
		MontageToPlay = GetReactMontage(ActorInfo->AvatarActor.Get());
	}
	if (!Anim || !MontageToPlay)
		return;

	// 解绑旧结束回调，避免停掉旧蒙太奇时触发 EndAbility 提前结束能力
	if (CurrentReactMontage)
	{
		FOnMontageEnded EmptyEndedDelegate;
		Anim->Montage_SetEndDelegate(EmptyEndedDelegate, CurrentReactMontage);
		// 旧动画用小过渡淡出，与新动画的淡入形成交叉过渡
		Anim->Montage_Stop(HitReactBlendTime, CurrentReactMontage);
	}

	// 按新的命中方向重播
	PlayReactMontage(Anim, MontageToPlay, Payload);
}

FGameplayTagContainer UFT_HitReactAbility::CollectTriggerEventTags() const
{
	FGameplayTagContainer Tags;
	for (const FAbilityTriggerData& Trigger : AbilityTriggers)
	{
		if (Trigger.TriggerSource == EGameplayAbilityTriggerSource::GameplayEvent)
		{
			Tags.AddTag(Trigger.TriggerTag);
		}
	}
	return Tags;
}

void UFT_HitReactAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
	// 移除播放期间的命中事件监听
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		const FGameplayTagContainer TriggerTags = CollectTriggerEventTags();
		if (!TriggerTags.IsEmpty())
		{
			ASC->RemoveGameplayEventTagContainerDelegate(TriggerTags, HitEventHandle);
		}
		HitEventHandle.Reset();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UFT_HitReactAbility::OnReactMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo)
		return;

	EndAbility(GetCurrentAbilitySpecHandle(), ActorInfo, GetCurrentActivationInfo(), true, bInterrupted);
}

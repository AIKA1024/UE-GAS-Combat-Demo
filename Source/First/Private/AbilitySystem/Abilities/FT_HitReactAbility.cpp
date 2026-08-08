#include "AbilitySystem/Abilities/FT_HitReactAbility.h"

#include "AbilitySystem/Combat/UFT_HitReactFunctionLibrary.h"
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
		Anim->Montage_Play(MontageToPlay, 1.f);

		if (IsValid(TriggerEventData->Instigator) && IsValid(TriggerEventData->Target))
		{
			const FVector Dir = UFT_HitReactFunctionLibrary::ComputeHitDirection(
				TriggerEventData->Instigator->GetActorLocation(),
				TriggerEventData->Target->GetActorLocation(),
				TriggerEventData->Target->GetActorRotation());
			const FName SectionName = UFT_HitReactFunctionLibrary::GetHitReactSectionNameByFVector(Dir);
			Anim->Montage_JumpToSection(SectionName);
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *SectionName.ToString()));
		}

		// 绑定该蒙太奇的结束回调：播放完/被打断都会触发 → 结束能力
		FOnMontageEnded EndedDelegate;
		EndedDelegate.BindUObject(this, &ThisClass::OnReactMontageEnded);
		Anim->Montage_SetEndDelegate(EndedDelegate, MontageToPlay);
	}
	else
	{
		// 没取到蒙太奇（角色没配/子类没回退）：仅执行打断/封锁逻辑后立即结束
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UFT_HitReactAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UFT_HitReactAbility::OnReactMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo)
		return;

	EndAbility(GetCurrentAbilitySpecHandle(), ActorInfo, GetCurrentActivationInfo(), true, bInterrupted);
}

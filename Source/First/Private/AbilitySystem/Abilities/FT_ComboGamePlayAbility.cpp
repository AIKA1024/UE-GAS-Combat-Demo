#include "AbilitySystem/Abilities/FT_ComboGamePlayAbility.h"

#include "AbilitySystem/Combat/UFT_ComboComponent.h"

void UFT_ComboGamePlayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ComboNodeTag.IsValid())
	{
		if (AActor* Avatar = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr)
		{
			if (UFT_ComboComponent* Combo = Avatar->FindComponentByClass<UFT_ComboComponent>())
				Combo->NotifyComboActivated(ComboNodeTag);
		}
	}
}

void UFT_ComboGamePlayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (ComboNodeTag.IsValid())
	{
		if (AActor* Avatar = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr)
		{
			if (UFT_ComboComponent* Combo = Avatar->FindComponentByClass<UFT_ComboComponent>())
				Combo->NotifyComboEnded(ComboNodeTag, bWasCancelled);
		}
	}
}

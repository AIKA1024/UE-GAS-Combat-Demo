#include "AbilitySystem/Abilities/FT_GamePlayAbility.h"

#include "AbilitySystemComponent.h"

void UFT_GamePlayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
		return;
	
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ActivationRequiredTags.IsEmpty())
		ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTags(ActivationRequiredTags);//模拟消耗掉ActivationRequiredTags的效果
	
	if (bDrawDebugs)
		GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Green,FString::Printf(TEXT("%s Activated!"),*GetName()));
}

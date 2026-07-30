// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/FT_GamePlayAbility.h"

void UFT_GamePlayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (bDrawDebugs)
		GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Green,FString::Printf(TEXT("%s Activated!"),*GetName()));
}

#include "AbilitySystem/FT_AbilitySystemComponent.h"

#include "GamePlayTags/FTTag.h"

void UFT_AbilitySystemComponent::OnChasingTagChanged(FGameplayTag GameplayTag, int I)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, GameplayTag.GetTagName().ToString());
}

void UFT_AbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	RegisterGameplayTagEvent(FTTag::Status::Chasing, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &UFT_AbilitySystemComponent::OnChasingTagChanged);
}

UFT_AbilitySystemComponent::UFT_AbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false; 
}

void UFT_AbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	HandleAutoActivatedAbility(AbilitySpec);
}

void UFT_AbilitySystemComponent::HandleAutoActivatedAbility(const FGameplayAbilitySpec& AbilitySpec)
{
	if (!AbilitySpec.Ability) return;

	for (const FGameplayTag& Tag : AbilitySpec.Ability->GetAssetTags())
	{
		if (Tag.MatchesTagExact(FTTag::Abilities::ActivatedOnGiven))
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

#include "AbilitySystem/FT_AbilitySystemComponent.h"

#include "GamePlayTags/FTTag.h"

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



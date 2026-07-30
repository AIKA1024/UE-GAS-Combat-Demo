#include "First/Public/Characters/FT_BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"

AFT_BaseCharacter::AFT_BaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* AFT_BaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

void AFT_BaseCharacter::GiveStartupAbilities()
{
	if (!GetAbilitySystemComponent())
		return;

	for (const auto& Ability : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(Ability);
		GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	}
}


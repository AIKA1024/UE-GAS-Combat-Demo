#include "First/Public/Player/FT_PlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/FT_AbilitySystemComponent.h"
#include "AbilitySystem/FT_AttributeSet.h"

AFT_PlayerState::AFT_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UFT_AbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UFT_AttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AFT_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

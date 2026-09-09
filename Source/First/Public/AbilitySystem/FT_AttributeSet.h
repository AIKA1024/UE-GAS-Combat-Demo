#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

#include "FT_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class FIRST_API UFT_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	//Health
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health = 100;
	ATTRIBUTE_ACCESSORS(ThisClass, Health);

	//MaxHealth
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth = 100;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth);

	//Mana
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Mana = 100;
	ATTRIBUTE_ACCESSORS(ThisClass, Mana);

	//MaxMana
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxMana = 100;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxMana);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Combat")
	FGameplayAttributeData AttackPower = 10;
	ATTRIBUTE_ACCESSORS(UFT_AttributeSet, AttackPower);
};

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
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(ThisClass, Health);

	//MaxHealth
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth);

	//Mana
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(ThisClass, Mana);

	//MaxMana
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxMana);

	//CurrentPoise 当前韧性：出招时由攻击 GA 授予，受击扣减；归零 = 韧性破，受击可打断施法
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData CurrentPoise;
	ATTRIBUTE_ACCESSORS(ThisClass, CurrentPoise);

	//MaxPoise 韧性上限（当前仅作为授予时钳制，预留）
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxPoise;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxPoise);
};

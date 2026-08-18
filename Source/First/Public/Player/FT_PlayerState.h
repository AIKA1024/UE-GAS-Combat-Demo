#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "FT_PlayerState.generated.h"

class UFT_AbilitySystemComponent;
class UFT_AttributeSet;

UCLASS()
class FIRST_API AFT_PlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFT_AttributeSet> AttributeSet;
	
	UPROPERTY(VisibleAnywhere,Category="First|Ability")
	TObjectPtr<UFT_AbilitySystemComponent> AbilitySystemComponent;

	AFT_PlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFT_AttributeSet* GetAttributeSet() const { return AttributeSet; }
};

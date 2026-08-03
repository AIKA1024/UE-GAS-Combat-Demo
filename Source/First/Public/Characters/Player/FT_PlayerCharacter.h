#pragma once

#include "CoreMinimal.h"
#include "First/Public/Characters/FT_BaseCharacter.h"
#include "FT_PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UFT_ComboComponent;

UCLASS()
class FIRST_API AFT_PlayerCharacter : public AFT_BaseCharacter
{
	GENERATED_BODY()

public:
	AFT_PlayerCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

	UFT_ComboComponent* GetComboComponent() const { return ComboComponent; }

private:
	UPROPERTY(VisibleAnywhere, Category=Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, Category="First|Combat")
	TObjectPtr<UFT_ComboComponent> ComboComponent;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "FT_PlayerController.generated.h"

struct FGameplayTag;
struct FInputActionValue;
class UInputAction;
class UFT_ComboComponent;

UCLASS()
class FIRST_API AFT_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="First|Input")
	TArray<TObjectPtr<UInputMappingContext>> InputMappingContexts;

	UPROPERTY(EditDefaultsOnly, Category="First|Input|Movement")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category="First|Input|Movement")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category="First|Input|Movement")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category="First|Input|Movement")
	TObjectPtr<UInputAction> RollAction;
	
	UPROPERTY(EditDefaultsOnly, Category="First|Input|Abilities")
	TObjectPtr<UInputAction> PrimaryAction;

	UPROPERTY(EditDefaultsOnly, Category="First|Input|Abilities")
	TObjectPtr<UInputAction> SecondaryAction;

	void Jump();
	void StopJumping();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Roll();
	void Primary();
	void Secondary();
	void ActivateAbility(const FGameplayTag& AbilityTag) const;
	UFT_ComboComponent* GetComboComponent() const;
};

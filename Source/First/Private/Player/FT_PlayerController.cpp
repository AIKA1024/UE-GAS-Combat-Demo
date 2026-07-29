#include "First/Public/Player/FT_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

class UEnhancedInputLocalPlayerSubsystem;

void AFT_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!InputSubsystem)
		return;

	for (const UInputMappingContext* Context : InputMappingContexts)
		InputSubsystem->AddMappingContext(Context, 0);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent) return;
	
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Triggered, this, &ThisClass::Primary);
	EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Started, this, &ThisClass::Secondary);
}

void AFT_PlayerController::Jump()
{
}

void AFT_PlayerController::StopJumping()
{
}

void AFT_PlayerController::Move(const FInputActionValue& Value)
{
}

void AFT_PlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void AFT_PlayerController::Primary()
{
}

void AFT_PlayerController::Secondary()
{
}

void AFT_PlayerController::ActivateAbility(const FGameplayTag& AbilityTag) const
{
}

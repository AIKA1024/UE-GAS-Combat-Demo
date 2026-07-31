#include "First/Public/Player/FT_PlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GamePlayTags/FTTag.h"

class UAbilitySystemComponent;
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
	EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Started, this, &ThisClass::Primary);
	EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Started, this, &ThisClass::Secondary);
}

void AFT_PlayerController::Jump()
{
	GetCharacter()->Jump();
}

void AFT_PlayerController::StopJumping()
{
	GetCharacter()->StopJumping();
}

void AFT_PlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	GetPawn()->AddMovementInput(ForwardVector, MovementVector.Y);
	GetPawn()->AddMovementInput(RightVector, MovementVector.X);
}

void AFT_PlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void AFT_PlayerController::Primary()
{
	ActivateAbility(FTTag::Abilities::Primary);
	if (bCanPerInput)
		PerInputTag = FTTag::Abilities::Primary;
}

void AFT_PlayerController::Secondary()
{
	ActivateAbility(FTTag::Abilities::Secondary);
	if (bCanPerInput)
		PerInputTag = FTTag::Abilities::Secondary;
}

void AFT_PlayerController::ActivateAbility(const FGameplayTag& AbilityTag) const
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
	if (!ASC || ASC->HasMatchingGameplayTag(FTTag::Status::Dead))
		return;

	ASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
}

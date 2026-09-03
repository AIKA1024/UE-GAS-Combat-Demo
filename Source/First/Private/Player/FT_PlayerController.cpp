#include "First/Public/Player/FT_PlayerController.h"

#include "AbilitySystem/Combat/UFT_ComboComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GamePlayTags/FTTag.h"

class IAbilitySystemInterface;
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
	EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &ThisClass::Roll);
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
	IAbilitySystemInterface* Interface = Cast<IAbilitySystemInterface>(GetPawn());
	auto ASC = Interface->GetAbilitySystemComponent();
	if (!ASC) return;
	
	if (ASC->HasMatchingGameplayTag(FTTag::Window::Cancelable))
	{
		FGameplayTagContainer AttackTags;
		AttackTags.AddTag(FTTag::Abilities::Primary);
		AttackTags.AddTag(FTTag::Abilities::Secondary);
		ASC->CancelAbilities(&AttackTags);
	}

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

void AFT_PlayerController::Roll()
{
	if (UFT_ComboComponent* Combo = GetComboComponent())
	{
		Combo->ClearBufferedAttack(); // 翻滚=放弃提前按下的攻击
		Combo->RefreshComboGrace(); // 翻滚延长连段保留时间，翻滚后攻击可继续连段
	}
	ActivateAbility(FTTag::Abilities::Roll);
}

void AFT_PlayerController::Primary()
{
	if (UFT_ComboComponent* Combo = GetComboComponent())
		Combo->RequestAttack(FTTag::Abilities::Primary);
}

void AFT_PlayerController::Secondary()
{
	if (UFT_ComboComponent* Combo = GetComboComponent())
		Combo->RequestAttack(FTTag::Abilities::Secondary);
}

void AFT_PlayerController::ActivateAbility(const FGameplayTag& AbilityTag) const
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
	if (!ASC || ASC->HasMatchingGameplayTag(FTTag::Status::Dead))
		return;

	ASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
}

UFT_ComboComponent* AFT_PlayerController::GetComboComponent() const
{
	if (APawn* CurrentPawn = GetPawn())
		return CurrentPawn->FindComponentByClass<UFT_ComboComponent>();
	return nullptr;
}

FGenericTeamId AFT_PlayerController::GetGenericTeamId() const
{
	// 如果 Character 实现了接口，直接返回 Character 的队伍 ID
	if (const IGenericTeamAgentInterface* CharAsTeam = Cast<IGenericTeamAgentInterface>(GetPawn()))
	{
		return CharAsTeam->GetGenericTeamId();
	}
	return FGenericTeamId::NoTeam;
}

ETeamAttitude::Type AFT_PlayerController::GetTeamAttitudeTowards(const AActor& Other) const
{
	// 委托给当前附身的 Character 去做态度判定
	if (const IGenericTeamAgentInterface* CharAsTeam = Cast<IGenericTeamAgentInterface>(GetPawn()))
	{
		return CharAsTeam->GetTeamAttitudeTowards(Other);
	}
	return ETeamAttitude::Neutral;
}

#include "First/Public/Characters/Player/FT_PlayerCharacter.h"

#include "AbilitySystem/Combat/UFT_ComboComponent.h"
#include "AbilitySystem/FT_AbilitySystemComponent.h"
#include "AbilitySystem/FT_AttributeSet.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Player/FT_PlayerState.h"

AFT_PlayerCharacter::AFT_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->bOrientRotationToMovement = true;
		MoveComp->RotationRate = FRotator(0.f, 540.f, 0.f);
	}

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetupAttachment(RootComponent);
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom);

	ComboComponent = CreateDefaultSubobject<UFT_ComboComponent>("ComboComponent");
	AIPerceptionStimuliSourceComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("AIPerceptionStimuliSourceComp");
}

UAbilitySystemComponent* AFT_PlayerCharacter::GetAbilitySystemComponent() const
{
	const AFT_PlayerState* FTPlayerState = Cast<AFT_PlayerState>(GetPlayerState());
	if (!FTPlayerState) return nullptr;
	
	return FTPlayerState->AbilitySystemComponent;
}

void AFT_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(),this);

	// GAS 不会自动注册挂在 PlayerState 上的 AttributeSet（只自动注册 ASC 的子对象），必须显式挂上
	if (AFT_PlayerState* PS = Cast<AFT_PlayerState>(GetPlayerState()))
	{
		if (UFT_AttributeSet* Attr = PS->GetAttributeSet())
			GetAbilitySystemComponent()->AddSpawnedAttribute(Attr);
	}

	GiveStartupAbilities();
}

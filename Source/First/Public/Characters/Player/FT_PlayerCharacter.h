#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "First/Public/Characters/FT_BaseCharacter.h"
#include "FT_PlayerCharacter.generated.h"

class UAIPerceptionStimuliSourceComponent;
class UCameraComponent;
class USpringArmComponent;
class UFT_ComboComponent;

UCLASS()
class FIRST_API AFT_PlayerCharacter : public AFT_BaseCharacter,public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AFT_PlayerCharacter();
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual void BeginPlay() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

	UFT_ComboComponent* GetComboComponent() const { return ComboComponent; }
	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(1); }

private:
	UPROPERTY(VisibleAnywhere, Category=Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, Category="First|Combat")
	TObjectPtr<UFT_ComboComponent> ComboComponent;
	
	UPROPERTY(VisibleAnywhere,Category="First|AI")
	TObjectPtr<UAIPerceptionStimuliSourceComponent> AIPerceptionStimuliSourceComp;
};

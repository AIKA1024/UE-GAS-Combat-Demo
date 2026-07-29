#pragma once

#include "CoreMinimal.h"
#include "First/Public/Characters/FT_BaseCharacter.h"
#include "FT_PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class FIRST_API AFT_PlayerCharacter : public AFT_BaseCharacter
{
	GENERATED_BODY()

public:
	AFT_PlayerCharacter();

private:
	UPROPERTY(VisibleAnywhere, Category=Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	TObjectPtr<UCameraComponent> FollowCamera;
};

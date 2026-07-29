// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FT_BaseCharacter.generated.h"

UCLASS()
class FIRST_API AFT_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFT_BaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
};

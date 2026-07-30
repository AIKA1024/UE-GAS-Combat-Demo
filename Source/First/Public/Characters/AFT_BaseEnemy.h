// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FT_BaseCharacter.h"
#include "AFT_BaseEnemy.generated.h"

class UFT_AbilitySystemComponent;

UCLASS()
class FIRST_API AFT_BaseEnemy : public AFT_BaseCharacter
{
	GENERATED_BODY()

public:
	AFT_BaseEnemy();
	virtual void BeginPlay() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
private:
	UPROPERTY(VisibleAnywhere,Category="First|Ability")
	TObjectPtr<UFT_AbilitySystemComponent> AbilitySystemComponent;
};

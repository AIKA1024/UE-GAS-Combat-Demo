// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "FT_BaseCharacter.generated.h"

class UGameplayAbility;

UCLASS()
class FIRST_API AFT_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFT_BaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	void GiveStartupAbilities();
	
private:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorWidget.h"
#include "EnemyHealthBar.generated.h"

class UAbilitySystemComponent;
class UProgressBar;
/**
 * 
 */
UCLASS()
class FIRST_API UEnemyHealthBar : public UActorWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget),BlueprintReadOnly)
	TObjectPtr<UProgressBar> HealthBar;

	UFUNCTION()
	void SetHealthPercent(float Percent) const;

private:
	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	void InitHealthBar(UAbilitySystemComponent* InASC);
	virtual void InitializeFromOwner() override;
};

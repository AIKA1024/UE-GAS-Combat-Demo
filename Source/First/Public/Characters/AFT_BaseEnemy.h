// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FT_BaseCharacter.h"
#include "GameplayEffectTypes.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/Combat/Config/CombatAnimationData.h"
#include "AFT_BaseEnemy.generated.h"

class UStateTreeComponent;
class UActorWidgetComponent;
class UWidgetComponent;
class AFT_PlayerCharacter;
class UAIPerceptionComponent;
class UFT_AbilitySystemComponent;
class UFT_AttributeSet;


UCLASS()
class FIRST_API AFT_BaseEnemy : public AFT_BaseCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AFT_BaseEnemy();
	void HandleHealthAttributeChange(const FOnAttributeChangeData& Data) const;
	void HandleMaxHealthAttributeChange(const FOnAttributeChangeData& Data) const;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(2); } // UAIPerceptionComponent队伍
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	FFT_CombatAnimationData CombatAnimationData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UActorWidgetComponent> HealthBarWidget;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UStateTreeComponent> StateTreeComp;

private:
	UPROPERTY(VisibleAnywhere, Category="First|Ability")
	TObjectPtr<UFT_AbilitySystemComponent> AbilitySystemComponent;

	/** 敌人属性集（Health/韧性等），与 ASC 同挂 Pawn 上 */
	UPROPERTY(VisibleAnywhere, Category="First|Ability")
	TObjectPtr<UFT_AttributeSet> AttributeSet;
	
	UPROPERTY()
	TObjectPtr<APlayerCameraManager> CameraManager;
	
	void UpdateHealthBarPercent() const;
};

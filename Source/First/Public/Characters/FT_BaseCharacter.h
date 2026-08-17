// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "FT_BaseCharacter.generated.h"

class UAnimMontage;
class UGameplayAbility;

UCLASS()
class FIRST_API AFT_BaseCharacter : public ACharacter, public IAbilitySystemInterface,public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	AFT_BaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** 完整受击蒙太奇（每个角色 BP 各自配自己骨骼的受击动画；受击 GA 从这里取） */
	UAnimMontage* GetStaggerMontage() const { return StaggerMontage; }

	/** 霸体附加受击蒙太奇（每个角色 BP 各自配） */
	UAnimMontage* GetArmorHitMontage() const { return ArmorHitMontage; }
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

protected:
	void GiveStartupAbilities();

	/** 受击硬直（State.HitReact）挂/摘时：挂上禁移动，摘下恢复 */
	void OnHitReactTagChanged(FGameplayTag Tag, int32 NewCount);

private:
	/** 启动能力（角色 BP 里配）。受击反应能力（GA_HitReact_Stagger/Armor 的 BP 子类）也放这里 */
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	/** 受击前缓存的移动模式，硬直结束后恢复 */
	EMovementMode CachedMovementMode = MOVE_Walking;

	/** 完整受击蒙太奇（细节面板可编辑，通过 GetStaggerMontage 读取） */
	UPROPERTY(EditDefaultsOnly, Category="First|HitReact")
	TObjectPtr<UAnimMontage> StaggerMontage;

	/** 霸体附加受击蒙太奇（细节面板可编辑，通过 GetArmorHitMontage 读取） */
	UPROPERTY(EditDefaultsOnly, Category="First|HitReact")
	TObjectPtr<UAnimMontage> ArmorHitMontage;
};

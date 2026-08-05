#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FT_GamePlayAbility.h"
#include "FT_HitReactAbility.generated.h"

class UAnimMontage;

/**
 * 受击反应 GA 基类：
 *  - 通过 AbilityTriggers(GameplayEvent) 由 Event.Hit.* 事件激活（子类构造里配）
 *  - 激活时播放 GetReactMontage() 选出的蒙太奇，蒙太奇结束/被打断时结束能力
 *  - 子类通过 AbilityTags / CancelAbilitiesWithTag / BlockAbilitiesWithTag 区分
 *    完整受击（打断施法 + 硬直封锁）与霸体附加受击（不打断、不封锁）
 *
 * 蒙太奇来源统一由子类 GetReactMontage() 提供（从角色身上的 StaggerMontage/ArmorHitMontage 取，
 * 每个角色 BP 配自己的受击动画，能力本身可全项目公用）。角色没配蒙太奇时，
 * 能力只完成打断/封锁逻辑后立即结束（骨架可跑）。
 */
UCLASS(Abstract)
class FIRST_API UFT_HitReactAbility : public UFT_GamePlayAbility
{
	GENERATED_BODY()

public:
	UFT_HitReactAbility();

protected:
	/** 选取本次要播的受击蒙太奇（子类实现：从角色身上取对应动画）；返回 nullptr = 不播动画只执行逻辑 */
	virtual UAnimMontage* GetReactMontage(const AActor* Avatar) const { return nullptr; }

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	/** 蒙太奇播放完毕/被打断 → 结束能力 */
	void OnReactMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};

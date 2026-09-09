#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FT_GamePlayAbility.h"
#include "FT_HitReactAbility.generated.h"

class UAnimMontage;
class UAnimInstance;

/**
 * 受击反应 GA 基类：
 *  - 通过 AbilityTriggers(GameplayEvent) 由 Event.Hit.* 事件激活（子类构造里配）
 *  - 激活时播放 GetReactMontage() 选出的蒙太奇，蒙太奇结束/被打断时结束能力
 *  - bRetriggerInstancedAbility=true：播放中再次命中时，引擎先结束当前激活再重新激活
 *    → 蒙太奇重播。重播时引擎新建蒙太奇实例并按组规则停掉旧实例，配合 BlendIn
 *    形成交叉过渡，无需自己监听命中事件
 *  - 子类通过 AbilityTags / CancelAbilitiesWithTag / BlockAbilitiesWithTag 决定
 *    受击是否打断施法、是否封锁动作
 *
 * 蒙太奇来源统一由子类 GetReactMontage() 提供（从角色身上的 StaggerMontage 取，
 * 每个角色 BP 配自己的受击动画，能力本身可全项目公用）。角色没配蒙太奇时，
 * 能力只完成打断/封锁逻辑后立即结束（骨架可跑）。
 */
UCLASS(Abstract)
class FIRST_API UFT_HitReactAbility : public UFT_GamePlayAbility
{
	GENERATED_BODY()

public:
	UFT_HitReactAbility();

	/** 受击动画重播时的过渡时长（旧动画淡出 + 新动画淡入），秒 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First|HitReact")
	float HitReactBlendTime{0.15f};

protected:
	/** 选取本次要播的受击蒙太奇（子类实现：从角色身上取对应动画）；返回 nullptr = 不播动画只执行逻辑 */
	virtual UAnimMontage* GetReactMontage(const AActor* Avatar) const { return nullptr; }

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** 蒙太奇播放完毕/被打断 → 结束能力（由 PlayReactMontage 里按令牌甄别后调用） */
	void OnReactMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	/** 播放蒙太奇 + 按命中方向跳节 + 绑定结束回调 */
	void PlayReactMontage(UAnimInstance* Anim, UAnimMontage* MontageToPlay, const FGameplayEventData* Payload);

private:
	/**
	 * 蒙太奇结束回调令牌：每次 PlayReactMontage 递增。
	 * 重触发时旧蒙太奇实例被打断 blend-out 后其结束回调会迟到，且回调只传蒙太奇资产指针
	 * （重触发前后是同一资产，无法靠指针甄别）→ 只有令牌等于当前值的回调才能结束能力
	 */
	uint64 ReactMontageToken = 0;
};

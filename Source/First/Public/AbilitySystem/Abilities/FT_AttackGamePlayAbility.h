#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FT_GamePlayAbility.h"
#include "GameplayTagContainer.h"
#include "FT_AttackGamePlayAbility.generated.h"

/**
 * 攻击招式基类（玩家连段招与敌人招式共用）：
 *  - 激活时自动携带 State.Attacking（供韧性判定与受击 GA 的 Cancel/Block 使用）
 *  - PoiseGranted > 0：授予韧性 CurrentPoise + 挂 SuperArmor（霸体）
 *  - 激活期间监听 AttackHit 事件，命中目标后做伤害 + 韧性判定
 * 子类在 BP 里配攻击蒙太奇（挂 FT_NormalAttackState 通知）与 PoiseGranted/PoiseDamage/Damage。
 */
UCLASS()
class FIRST_API UFT_AttackGamePlayAbility : public UFT_GamePlayAbility
{
	GENERATED_BODY()

public:
	UFT_AttackGamePlayAbility();

	/** 本招韧性值（霸体值）：>0 时出招期间韧性未打空前受击不打断施法。0 = 无霸体，被打即断 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First|Poise")
	float PoiseGranted = 0.f;

	/** 本招每击的韧性伤害：命中目标后从目标韧性中扣除 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First|Poise")
	float PoiseDamage = 0.f;

	/** 本招每击的生命伤害（占位：后续建议替换为正式伤害 GameplayEffect + 减伤管线） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First|Combat")
	float Damage = 0.f;

protected:
	/** 正式伤害结算的 GameplayEffect 类型 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "First|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	/** 命中事件回调（AttackHit）：对命中的目标做伤害 + 韧性判定 */
	void HandleAttackHitEvent(FGameplayTag EventTag, const FGameplayEventData* Payload);

private:
	/** 激活期间绑定的命中事件句柄（EndAbility 时解绑） */
	FDelegateHandle AttackHitEventHandle;

	/**
	 * 本次激活已结算过（伤害+受击）的目标：同一目标在一次攻击激活内只结算一次。
	 * 合并同帧重复 AttackHit 的来源：单次 sweep 多组件命中（胶囊体+网格）、多 Mesh 命中同一目标等。
	 * 多段攻击应拆成多次 GA 激活（与连段设计一致：每段连击一个 GA）。
	 */
	TSet<TWeakObjectPtr<AActor>> SettledTargets;
};

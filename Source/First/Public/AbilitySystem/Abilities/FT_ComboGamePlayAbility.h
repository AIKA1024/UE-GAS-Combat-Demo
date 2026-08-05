#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FT_AttackGamePlayAbility.h"
#include "GameplayTagContainer.h"
#include "FT_ComboGamePlayAbility.generated.h"

/**
 * 连段特化的 GameplayAbility（玩家侧）：
 *  - 继承 UFT_AttackGamePlayAbility：攻击/韧性/伤害/命中判定逻辑都在基类
 *  - 额外：激活/结束时通知 UFT_ComboComponent 更新连段状态
 * ComboNodeTag 标识本招在连段树里的节点（须与树里该节点的 NodeTag 一致）。
 */
UCLASS()
class FIRST_API UFT_ComboGamePlayAbility : public UFT_AttackGamePlayAbility
{
	GENERATED_BODY()

public:
	/** 连段节点 tag（与连段树里该节点的 NodeTag 一致） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First|Combo")
	FGameplayTag ComboNodeTag;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};

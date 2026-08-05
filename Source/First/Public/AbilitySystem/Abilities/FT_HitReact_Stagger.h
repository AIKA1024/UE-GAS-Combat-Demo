#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FT_HitReactAbility.h"
#include "FT_HitReact_Stagger.generated.h"

/**
 * 完整受击（普通受击 / 韧性破）：
 *  - 监听 Event.Hit.Normal 与 Event.Hit.Break
 *  - 激活时取消目标正在播放的攻击（CancelAbilitiesWithTag=Attacking）→ 打断施法
 *  - 硬直期间封锁攻击与翻滚（BlockAbilitiesWithTag）
 */
UCLASS()
class FIRST_API UFT_HitReact_Stagger : public UFT_HitReactAbility
{
	GENERATED_BODY()

public:
	UFT_HitReact_Stagger();

protected:
	/** 优先用角色身上的完整受击蒙太奇（各角色 BP 各自配），没有才回退能力自身 */
	virtual UAnimMontage* GetReactMontage(const AActor* Avatar) const override;
};

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FT_HitReactAbility.h"
#include "FT_HitReact_Armor.generated.h"

/**
 * 霸体附加受击：
 *  - 监听 Event.Hit.Armor（韧性被打但未打空）
 *  - 不 Cancel / 不 Block：不打断施法、不封锁动作，仅播放附加受击动画
 */
UCLASS()
class FIRST_API UFT_HitReact_Armor : public UFT_HitReactAbility
{
	GENERATED_BODY()

public:
	UFT_HitReact_Armor();

protected:
	/** 优先用角色身上的霸体附加受击蒙太奇（各角色 BP 各自配），没有才回退能力自身 */
	virtual UAnimMontage* GetReactMontage(const AActor* Avatar) const override;
};

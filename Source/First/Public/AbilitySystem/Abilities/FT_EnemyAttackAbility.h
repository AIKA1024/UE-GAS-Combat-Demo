#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FT_AttackGamePlayAbility.h"
#include "FT_EnemyAttackAbility.generated.h"

/**
 * 敌人攻击招式骨架：
 *  - 一套招式 = 一个 GA（多段蒙太奇 + 段推进），AI 激活一次打满整套；
 *    韧性破/受击打断时整个 GA 被取消
 *  - 继承 UFT_AttackGamePlayAbility：自动挂 State.Attacking、PoiseGranted 霸体、命中伤害/韧性判定
 *
 * 用法：建 BP 子类
 *  1) 配攻击蒙太奇（多段用 section/通知推进），蒙太奇里挂 FT_Serath_PrimaryAttackState 通知做命中
 *  2) 填 PoiseGranted（有霸体的招式）、PoiseDamage、Damage
 *  3) AssetTags 里加一个 Ability.EnemyAttack 之类的自定义 tag，AI 用 TryActivateAbilityByTag 激活
 */
UCLASS()
class FIRST_API UFT_EnemyAttackAbility : public UFT_AttackGamePlayAbility
{
	GENERATED_BODY()
};

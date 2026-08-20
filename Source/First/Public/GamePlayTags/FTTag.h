#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace FTTag
{
    namespace Window
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combo)
    }
    
    namespace Combo
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(X)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(XX)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(XXX)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(XXXX)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(XY)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(XYX)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(XYY)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(XYYX)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Y)
    }

    namespace Abilities
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActivatedOnGiven)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(BlockHitReact)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Primary)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Secondary)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Roll)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Death)
    }

    namespace Status
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dead)

        /** 处于攻击招式播放中（攻击 GA 激活时自动挂上） */
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attacking)

        /** 霸体：韧性 > 0 时挂上，受击不被打断（播附加受击） */
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(SuperArmor)

        /** 正在硬直受击（完整受击/打断施法后） */
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact)

        /** 正在播放霸体附加受击 */
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(ArmorHit)

        // ========== AI 行为状态（新增） ==========
        
        /** AI正在战斗中（追击或攻击玩家） */
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combat)
        
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Chasing)     // 追击

        /** AI正在调查声音/可疑位置 */
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Investigate)

        /** AI正在巡逻 */
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Patrol)
    }

    namespace Events
    {
        /** 通用攻击命中事件标签（玩家和敌人共用） */
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(AttackHit)

        namespace Hit
        {
            /** 普通受击：目标未处于霸体（未出招或韧性为 0） */
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Normal)

            /** 霸体附加受击：韧性被扣但未归零 */
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Armor)

            /** 韧性破：韧性被这一击打空，打断施法 + 完整受击 */
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Break)
        }
    }

    namespace Data
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage)
    }
}
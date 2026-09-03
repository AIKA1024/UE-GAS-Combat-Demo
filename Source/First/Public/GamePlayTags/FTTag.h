#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace FTTag
{
    namespace Window
    {
       UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combo)
       UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cancelable)
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
       UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attacking)
       UE_DECLARE_GAMEPLAY_TAG_EXTERN(SuperArmor)
       UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact)
       UE_DECLARE_GAMEPLAY_TAG_EXTERN(ArmorHit)

       // ========== AI 运行时行为状态 ==========
       UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combat)
       UE_DECLARE_GAMEPLAY_TAG_EXTERN(Chasing)
       UE_DECLARE_GAMEPLAY_TAG_EXTERN(Investigate)
       UE_DECLARE_GAMEPLAY_TAG_EXTERN(Patrol)
       UE_DECLARE_GAMEPLAY_TAG_EXTERN(BlockMovement)
    }

    namespace AI
    {
       namespace Event
       {
          // -------------------------------------------------------------------------
          // 1. 明确威胁/直接战斗组：StateTree 监听父级 (Combat) 即可直接切入追击战斗
          // -------------------------------------------------------------------------
          namespace Combat
          {
             UE_DECLARE_GAMEPLAY_TAG_EXTERN(SightSpotted)       // 视野看到敌人
             UE_DECLARE_GAMEPLAY_TAG_EXTERN(HostileNoise)        // 听到开火/破门/呼喊等敌对声音
             UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageDirect)        // 受到伤害刺激（不论近战/远程）
             UE_DECLARE_GAMEPLAY_TAG_EXTERN(BumpedThreat)        // 背后碰撞/触碰到敌人
          }

          // -------------------------------------------------------------------------
          // 2. 弱威胁/调查组：StateTree 监听父级 (Investigate) 前往该坐标点警戒排查
          // -------------------------------------------------------------------------
          namespace Investigate
          {
             UE_DECLARE_GAMEPLAY_TAG_EXTERN(SuspiciousNoise)     // 听到碎石/脚步等可疑声响
             UE_DECLARE_GAMEPLAY_TAG_EXTERN(PredictionArrival)   // 预测感知完成，到达预判脱战点
          }

          // -------------------------------------------------------------------------
          // 3. 目标丢失/搜寻组：StateTree 监听父级 (Lost) 进入最后已知点搜寻或脱战
          // -------------------------------------------------------------------------
          namespace Lost
          {
             UE_DECLARE_GAMEPLAY_TAG_EXTERN(SightLost)           // 目标脱离视野/进入掩体
          }
       }
    }

    namespace Events
    {
       UE_DECLARE_GAMEPLAY_TAG_EXTERN(AttackHit)

       namespace Hit
       {
          UE_DECLARE_GAMEPLAY_TAG_EXTERN(Normal)
          UE_DECLARE_GAMEPLAY_TAG_EXTERN(Armor)
          UE_DECLARE_GAMEPLAY_TAG_EXTERN(Break)
       }
    }

    namespace Data
    {
       UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage)
    }
}
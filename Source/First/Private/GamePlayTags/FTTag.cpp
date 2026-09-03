#include "GamePlayTags/FTTag.h"

namespace FTTag
{
    namespace Window
    {
       UE_DEFINE_GAMEPLAY_TAG(Combo, "FFTag.Windows.Combo")
       UE_DEFINE_GAMEPLAY_TAG(Cancelable, "FFTag.Windows.Cancelable")
    }

    namespace Combo
    {
       UE_DEFINE_GAMEPLAY_TAG(X, "FFTag.Combo.X")
       UE_DEFINE_GAMEPLAY_TAG(XX, "FFTag.Combo.XX")
       UE_DEFINE_GAMEPLAY_TAG(XXX, "FFTag.Combo.XXX")
       UE_DEFINE_GAMEPLAY_TAG(XXXX, "FFTag.Combo.XXXX")
       UE_DEFINE_GAMEPLAY_TAG(XY, "FFTag.Combo.XY")
       UE_DEFINE_GAMEPLAY_TAG(XYX, "FFTag.Combo.XYX")
       UE_DEFINE_GAMEPLAY_TAG(XYY, "FFTag.Combo.XYY")
       UE_DEFINE_GAMEPLAY_TAG(XYYX, "FFTag.Combo.XYYX")
       UE_DEFINE_GAMEPLAY_TAG(Y, "FFTag.Combo.Y")
    }

    namespace Abilities
    {
       UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActivatedOnGiven, "FFTag.Abilities.ActivatedOnGiven", "Tag for Abilities Immediately once given.")
       UE_DEFINE_GAMEPLAY_TAG_COMMENT(BlockHitReact, "FFTag.Abilities.BlockHitReact", "Tag for BlockHitReact")
       UE_DEFINE_GAMEPLAY_TAG_COMMENT(Primary, "FFTag.Abilities.Primary", "Tag for the Primary Ability")
       UE_DEFINE_GAMEPLAY_TAG_COMMENT(Secondary, "FFTag.Abilities.Secondary", "Tag for the Secondary Ability")
       UE_DEFINE_GAMEPLAY_TAG_COMMENT(Roll, "FFTag.Abilities.Roll", "Tag for the Roll Ability")
       UE_DEFINE_GAMEPLAY_TAG_COMMENT(Death, "FFTag.Abilities.Death", "Tag for the Death Ability")
    }

    namespace Status
    {
       UE_DEFINE_GAMEPLAY_TAG_COMMENT(Dead, "FFTag.Status.Dead", "Tag for tag Dead Actor")
       UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attacking, "FFTag.Status.Attacking", "Tag for Attacking")
       UE_DEFINE_GAMEPLAY_TAG_COMMENT(SuperArmor, "FFTag.Status.SuperArmor", "Tag for SuperArmor")
       UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "FFTag.Status.HitReact", "Tag for HitReact")
       UE_DEFINE_GAMEPLAY_TAG_COMMENT(ArmorHit, "FFTag.Status.ArmorHit", "Tag for ArmorHit")

       // ========== AI 行为状态 ==========
       UE_DEFINE_GAMEPLAY_TAG(Combat, "FFTag.Status.Combat")
       UE_DEFINE_GAMEPLAY_TAG(Chasing, "FFTag.Status.Chasing")
       UE_DEFINE_GAMEPLAY_TAG(Investigate, "FFTag.Status.Investigate")
       UE_DEFINE_GAMEPLAY_TAG(Patrol, "FFTag.Status.Patrol")
       UE_DEFINE_GAMEPLAY_TAG(BlockMovement, "FFTag.Status.BlockMovement")
    }

    namespace AI
    {
       namespace Event
       {
          namespace Combat
          {
             UE_DEFINE_GAMEPLAY_TAG_COMMENT(SightSpotted, "FFTag.AI.Event.Combat.SightSpotted", "首次确认看见敌人/进入视线")
             UE_DEFINE_GAMEPLAY_TAG_COMMENT(HostileNoise, "FFTag.AI.Event.Combat.HostileNoise", "听到敌对声响（交火、破门等），直接触发战斗")
             UE_DEFINE_GAMEPLAY_TAG_COMMENT(DamageDirect, "FFTag.AI.Event.Combat.DamageDirect", "受到直接伤害刺激，直接触发战斗反应")
             UE_DEFINE_GAMEPLAY_TAG_COMMENT(BumpedThreat, "FFTag.AI.Event.Combat.BumpedThreat", "背后被目标碰撞或触碰，直接引发反击")
          }

          namespace Investigate
          {
             UE_DEFINE_GAMEPLAY_TAG_COMMENT(SuspiciousNoise, "FFTag.AI.Event.Investigate.SuspiciousNoise", "可疑轻微声响，触发前往位置调查")
             UE_DEFINE_GAMEPLAY_TAG_COMMENT(PredictionArrival, "FFTag.AI.Event.Investigate.PredictionArrival", "预测感知计算完毕，前往预判位置排查")
          }

          namespace Lost
          {
             UE_DEFINE_GAMEPLAY_TAG_COMMENT(SightLost, "FFTag.AI.Event.Lost.SightLost", "目标脱离视野，触发搜寻或脱战判定")
          }
       }
    }

    namespace Events
    {
       namespace Hit
       {
          UE_DEFINE_GAMEPLAY_TAG_COMMENT(Normal, "FFTag.Event.Hit.Normal", "Tag for Normal Hit")
          UE_DEFINE_GAMEPLAY_TAG_COMMENT(Armor, "FFTag.Event.Hit.Armor", "Tag for Armor Hit")
          UE_DEFINE_GAMEPLAY_TAG_COMMENT(Break, "FFTag.Event.Hit.Break", "Tag for Poise Break Hit")
       }

       UE_DEFINE_GAMEPLAY_TAG_COMMENT(AttackHit, "FFTag.Event.AttackHit", "Tag for Attack Hit Event (shared by player and enemy)")
    }

    namespace Data
    {
       UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage, "FFTag.Data.Damage", "Used in Set by Caller to pass base damage value.")
    }
}
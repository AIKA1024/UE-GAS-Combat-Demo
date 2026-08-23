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
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActivatedOnGiven, "FFTag.Abilities.ActivatedOnGiven",
		                               "Tag for Abilities Immediately once given.")

		UE_DEFINE_GAMEPLAY_TAG_COMMENT(BlockHitReact, "FFTag.Abilities.BlockHitReact",
		                               "Tag for BlockHitReact")

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

		// ========== AI 行为状态（新增） ==========
		UE_DEFINE_GAMEPLAY_TAG(FTTag::Status::Combat, "FFTag.Status.Combat");
		UE_DEFINE_GAMEPLAY_TAG(FTTag::Status::Chasing, "FFTag.Status.Chasing");
		UE_DEFINE_GAMEPLAY_TAG(FTTag::Status::Investigate, "FFTag.Status.Investigate");
		UE_DEFINE_GAMEPLAY_TAG(FTTag::Status::Patrol, "FFTag.Status.Patrol");
		UE_DEFINE_GAMEPLAY_TAG(FTTag::Status::BlockMovement, "FFTag.Status.BlockMovement");
	}

	namespace Events
	{
		namespace Hit
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Normal, "FFTag.Event.Hit.Normal", "Tag for Normal Hit")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Armor, "FFTag.Event.Hit.Armor", "Tag for Armor Hit")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Break, "FFTag.Event.Hit.Break", "Tag for Poise Break Hit")
		}

		// 通用攻击命中事件标签（玩家和敌人共用）
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(AttackHit, "FFTag.Event.AttackHit",
		                               "Tag for Attack Hit Event (shared by player and enemy)")
	}

	namespace Data
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage, "FFTag.Data.Damage",
		                               "Used in Set by Caller to pass base damage value. Input should be positive. The final attribute to be deducted (Health/Mana/Stamina, etc.) is determined by the GE's Modifier or Calculation Class. Note: This tag is for data passing ONLY, NOT for state marking. Do NOT use it as a blocking tag for Ability or State.")
	}
}

#include "GamePlayTags/FTTag.h"

namespace FTTag
{
	namespace Abilities
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActivatedOnGiven, "FFTag.Abilities.ActivatedOnGiven",
									   "Tag for Abilities Immediately once given.")
		
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(BlockHitReact, "FFTag.Abilities.BlockHitReact",
									   "Tag for BlockHitReact")
		
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Primary, "FFTag.Abilities.Primary", "Tag for the Primary Ability")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Secondary, "FFTag.Abilities.Secondary", "Tag for the Secondary Ability")

		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Death, "FFTag.Abilities.Death", "Tag for the Death Ability")
	}

	namespace Status
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Dead, "FFTag.Status.Dead", "Tag for tag Dead Actor")
		
	}
	namespace Events
	{
		namespace Player
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(PrimaryTraceHit, "FFTag.Event.Player.PrimaryTraceHit", "Tag for the PrimaryTraceHit of Player")
		}
	}
}

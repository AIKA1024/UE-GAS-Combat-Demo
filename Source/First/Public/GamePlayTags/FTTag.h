#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace FTTag
{
	namespace Abilities
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActivatedOnGiven)

		UE_DECLARE_GAMEPLAY_TAG_EXTERN(BlockHitReact)

		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Primary)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Secondary)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Death)
	}

	namespace Status
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dead)
	}

	namespace Events
	{
		namespace Player
		{
			namespace Attack
			{
				namespace ComboWindow
				{
					UE_DECLARE_GAMEPLAY_TAG_EXTERN(Open)
					UE_DECLARE_GAMEPLAY_TAG_EXTERN(Close)
				}
			}
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(PrimaryTraceHit)
		}
	}
}

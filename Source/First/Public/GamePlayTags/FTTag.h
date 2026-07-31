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
	}

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
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(PrimaryTraceHit)
		}
	}
}

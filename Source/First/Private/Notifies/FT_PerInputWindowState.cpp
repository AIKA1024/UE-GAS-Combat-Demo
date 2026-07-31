#include "Notifies/FT_PerInputWindowState.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NativeGameplayTags.h"
#include "Player/FT_PlayerController.h"

///UFT_PerInputWindow在动画中结尾时必须已经进入ComboWindow
void UFT_PerInputWindowState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AFT_PlayerController* Controller = Cast<AFT_PlayerController>(MeshComp->GetOwner()->GetInstigatorController()))
	{
		Controller->bCanPerInput = true;
		Controller->PerInputTag = FGameplayTag::EmptyTag;
	}
}

void UFT_PerInputWindowState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (AFT_PlayerController* Controller = Cast<AFT_PlayerController>(MeshComp->GetOwner()->GetInstigatorController()))
	{
		Controller->bCanPerInput = false;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(),Controller->PerInputTag,FGameplayEventData());
	}
}

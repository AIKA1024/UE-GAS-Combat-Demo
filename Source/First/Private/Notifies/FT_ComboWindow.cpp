#include "Notifies/FT_ComboWindow.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GamePlayTags/FTTag.h"

void UFT_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(),
	                                                         FTTag::Events::Player::Attack::ComboWindow::Open,
	                                                         FGameplayEventData());
}

void UFT_ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(),
	                                                         FTTag::Events::Player::Attack::ComboWindow::Close,
	                                                         FGameplayEventData());
}

#include "Notifies/FT_ComboWindow.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GamePlayTags/FTTag.h"

void UFT_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	const FGameplayTag ComboWindowTag = FTTag::Window::Combo;
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(MeshComp->GetOwner(), ComboWindowTag.GetSingleTagContainer());
}

void UFT_ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	const FGameplayTag ComboWindowTag = FTTag::Window::Combo;
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(MeshComp->GetOwner(),
	                                                        ComboWindowTag.GetSingleTagContainer());
}

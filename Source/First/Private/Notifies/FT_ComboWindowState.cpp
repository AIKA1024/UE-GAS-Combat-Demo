#include "Notifies/FT_ComboWindowState.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GamePlayTags/FTTag.h"

void UFT_ComboWindowState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	const FGameplayTag ComboWindowTag = FTTag::Window::Combo;
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(MeshComp->GetOwner(), ComboWindowTag.GetSingleTagContainer());
}

void UFT_ComboWindowState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	const FGameplayTag ComboWindowTag = FTTag::Window::Combo;
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(MeshComp->GetOwner(),
	                                                        ComboWindowTag.GetSingleTagContainer());
}

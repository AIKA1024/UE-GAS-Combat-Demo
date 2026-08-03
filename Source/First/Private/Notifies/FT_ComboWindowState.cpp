#include "Notifies/FT_ComboWindowState.h"

#include "AbilitySystem/Combat/UFT_ComboComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GamePlayTags/FTTag.h"

void UFT_ComboWindowState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	const FGameplayTag ComboWindowTag = FTTag::Window::Combo;
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(MeshComp->GetOwner(), ComboWindowTag.GetSingleTagContainer());

	// 先挂上窗口 tag，再通知连段组件触发缓冲的下一段（RequestComboAttack 需要看到窗口 tag）
	if (UFT_ComboComponent* Combo = MeshComp->GetOwner()->FindComponentByClass<UFT_ComboComponent>())
		Combo->NotifyComboWindowOpened();
}

void UFT_ComboWindowState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	const FGameplayTag ComboWindowTag = FTTag::Window::Combo;
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(MeshComp->GetOwner(),
	                                                        ComboWindowTag.GetSingleTagContainer());

	if (UFT_ComboComponent* Combo = MeshComp->GetOwner()->FindComponentByClass<UFT_ComboComponent>())
		Combo->NotifyComboWindowClosed();
}

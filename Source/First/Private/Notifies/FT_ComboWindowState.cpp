#include "Notifies/FT_ComboWindowState.h"

#include "AbilitySystem/Combat/UFT_ComboComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GamePlayTags/FTTag.h"

void UFT_ComboWindowState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float TotalDuration,
                                       const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	// 编辑器预览窗口中 Owner 为 nullptr，直接退出
	if (!Owner) return;

	const FGameplayTag ComboWindowTag = FTTag::Window::Combo;
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(Owner, ComboWindowTag.GetSingleTagContainer());

	// 先挂上窗口 tag，再通知连段组件触发缓冲的下一段
	if (UFT_ComboComponent* Combo = Owner->FindComponentByClass<UFT_ComboComponent>())
	{
		Combo->NotifyComboWindowOpened();
	}
}

void UFT_ComboWindowState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	const FGameplayTag ComboWindowTag = FTTag::Window::Combo;
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(MeshComp->GetOwner(),
	                                                        ComboWindowTag.GetSingleTagContainer());

	AActor* Owner = MeshComp->GetOwner();
	// 编辑器预览窗口中 Owner 为 nullptr，直接退出
	if (!Owner) return;
	if (UFT_ComboComponent* Combo = Owner->FindComponentByClass<UFT_ComboComponent>())
		Combo->NotifyComboWindowClosed();
}

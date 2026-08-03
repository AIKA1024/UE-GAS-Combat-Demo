#include "Notifies/FT_PerInputWindowState.h"

#include "AbilitySystem/Combat/UFT_ComboComponent.h"

///预输入窗口：只负责设置"接受按键"标志。
///窗口内提前按下的攻击由 UFT_ComboComponent 缓冲，连段窗口打开或本招收招时触发下一段。
void UFT_PerInputWindowState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (UFT_ComboComponent* Combo = MeshComp->GetOwner()->FindComponentByClass<UFT_ComboComponent>())
		Combo->SetPerInputOpen(true);
}

void UFT_PerInputWindowState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (UFT_ComboComponent* Combo = MeshComp->GetOwner()->FindComponentByClass<UFT_ComboComponent>())
		Combo->SetPerInputOpen(false);
}

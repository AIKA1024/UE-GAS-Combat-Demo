// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/FT_CancelWindow.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GamePlayTags/FTTag.h"

void UFT_CancelWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (const IAbilitySystemInterface* Owner = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
		if (const auto ASC = Owner->GetAbilitySystemComponent())
			ASC->AddLooseGameplayTag(FTTag::Window::Cancelable);
}

void UFT_CancelWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (const IAbilitySystemInterface* Owner = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
		if (const auto ASC = Owner->GetAbilitySystemComponent())
			ASC->RemoveLooseGameplayTag(FTTag::Window::Cancelable);
}

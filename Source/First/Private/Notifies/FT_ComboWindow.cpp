// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/FT_ComboWindow.h"

#include "AbilitySystemBlueprintLibrary.h"

void UFT_ComboWindow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(MeshComp->GetOwner(),ComboWindowTag.GetSingleTagContainer());
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(MeshComp->GetOwner(),ComboWindowTag.GetSingleTagContainer());
}

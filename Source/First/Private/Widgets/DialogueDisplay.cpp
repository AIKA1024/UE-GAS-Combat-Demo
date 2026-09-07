// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/DialogueDisplay.h"

#include "Components/AudioComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Dialogue/Dialogue.h"
#include "Kismet/GameplayStatics.h"

void UDialogueDisplay::SetNewDialogue(const FDialogue& Dialogue)
{
	if (IsValid(AudioComponent))
	{
		AudioComponent->Stop();
		AudioComponent = nullptr;
	}

	if (!Dialogue.Avatar.IsNull())
		if (UTexture2D* LoadedTexture = Dialogue.Avatar.LoadSynchronous())
			AvatarImage->SetBrushFromTexture(LoadedTexture, false);

	if (!Dialogue.Name.IsEmptyOrWhitespace())
		NameTextBlock->SetText(Dialogue.Name);

	ContentTextBlock->SetText(Dialogue.Text);

	if (!Dialogue.Sound.IsNull())
		if (USoundBase* Sound = Dialogue.Sound.LoadSynchronous())
			AudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), Sound);
}

void UDialogueDisplay::ProcessDialogue()
{
	if (!DialogueDataTable)
		return;

	if (FDialogue* DialoguePtr = GetNextDialogue())
	{
		FDialogue& Dialogue = *DialoguePtr;
		SetNewDialogue(Dialogue);
	}
	else
		DialogueFinish.Broadcast();
}

FDialogue* UDialogueDisplay::GetNextDialogue()
{
	auto RowNames = DialogueDataTable->GetRowNames();
	if (CurrentIndex >= RowNames.Num()) return nullptr;
	const FName RowName = RowNames[CurrentIndex];
	CurrentIndex++;
	return DialogueDataTable->FindRow<FDialogue>(RowName,TEXT("Context"));
}

void UDialogueDisplay::NativeConstruct()
{
	Super::NativeConstruct();

	ProcessDialogue();
}

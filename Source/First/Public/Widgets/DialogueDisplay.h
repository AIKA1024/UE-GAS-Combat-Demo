// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueDisplay.generated.h"

struct FDialogue;
class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueFinishSignature);

UCLASS()
class FIRST_API UDialogueDisplay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnDialogueFinishSignature DialogueFinish;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	TObjectPtr<UImage> AvatarImage;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	TObjectPtr<UTextBlock> NameTextBlock;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	TObjectPtr<UTextBlock> ContentTextBlock;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	TObjectPtr<UTextBlock> ContinueHintBlock;

	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> AudioComponent;

	int CurrentIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UDataTable> DialogueDataTable;
	
	void SetNewDialogue(const FDialogue& Dialogue);

	UFUNCTION(BlueprintCallable)
	void ProcessDialogue();
	
	FDialogue* GetNextDialogue();

protected:
	virtual void NativeConstruct() override;
};

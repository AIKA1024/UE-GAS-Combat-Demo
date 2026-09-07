#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Dialogue.generated.h"

class UTexture2D;
class USoundBase;

USTRUCT(BlueprintType)
struct FDialogue : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Avatar;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = true))
	FText Text;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USoundBase> Sound;
};

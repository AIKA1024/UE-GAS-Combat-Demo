#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GamePlayTags/FTTag.h"
#include "FT_ComboWindow.generated.h"

struct FGameplayTag;

UCLASS()
class FIRST_API UFT_ComboWindow : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FGameplayTag ComboWindowTag = FTTag::Window::Combo;
};

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "UFT_ComboDefinition.generated.h"

class UFT_ComboGamePlayAbility;

/** 连段树中的一个节点：本招 GA + 按输入去下一节点 */
USTRUCT(BlueprintType)
struct FFT_ComboNode
{
	GENERATED_BODY()

	/** 节点标识（须与对应 GA 的 ComboNodeTag 一致） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combo")
	FGameplayTag NodeTag;

	/** 本招 GA（激活后播放其蒙太奇） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combo")
	TSubclassOf<UFT_ComboGamePlayAbility> Ability;

	/** 输入 tag（FFTag.Abilities.Primary/Secondary）→ 下一节点 tag */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combo")
	TMap<FGameplayTag, FGameplayTag> NextByInput;
};

/**
 * 连段树定义（每角色一个实例）。
 * UFT_ComboComponent 按"当前节点 tag + 输入 tag"查树跳下一段。
 */
UCLASS()
class FIRST_API UFT_ComboDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	/** 输入 tag（Primary/Secondary）→ 起手节点 tag；支持左键起 X、右键起 Y 等多起手 */
	UPROPERTY(EditAnywhere, Category="Combo")
	TMap<FGameplayTag, FGameplayTag> StartNodesByInput;

	/** 所有节点（NodeTag 唯一） */
	UPROPERTY(EditAnywhere, Category="Combo")
	TArray<FFT_ComboNode> Nodes;

	/** 按 tag 查节点 */
	const FFT_ComboNode* FindNode(const FGameplayTag& NodeTag) const;
};

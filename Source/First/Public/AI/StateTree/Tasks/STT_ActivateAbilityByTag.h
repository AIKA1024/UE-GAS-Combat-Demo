#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StateTreeTaskBase.h"
#include "STT_ActivateAbilityByTag.generated.h"
class AAIController;

USTRUCT()
struct FIRST_API FSTT_ActivateAbilityByTagInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AActor> Actor = nullptr;

	/** 需要触发的技能标签（如 FFTag.Abilities.Primary） */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	FGameplayTag AbilityTag;

	/** 标识 Ability 是否已结束 */
	UPROPERTY(Transient)
	bool bAbilityEnded = false;
	
	/** 标记是否成功激活了 Ability */
	UPROPERTY(Transient)
	bool bActivatedSuccessfully = false;
	
	/** 保存委托句柄以便在 ExitState 时解绑 */
	FDelegateHandle AbilityEndedDelegateHandle;
};

USTRUCT(meta = (DisplayName = "Activate Ability By Tag (Wait End)", Category = "AI|GAS"))
struct FIRST_API FSTT_ActivateAbilityByTag : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTT_ActivateAbilityByTagInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
	                                       const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context,
	                       const FStateTreeTransitionResult& Transition) const override;
};

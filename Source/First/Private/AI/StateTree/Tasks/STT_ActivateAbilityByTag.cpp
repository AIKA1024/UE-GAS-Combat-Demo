#include "AI/StateTree/Tasks/STT_ActivateAbilityByTag.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FSTT_ActivateAbilityByTag::EnterState(FStateTreeExecutionContext& Context,
                                                          const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	InstanceData.bAbilityEnded = false;
	InstanceData.bActivatedSuccessfully = false;

	if (!InstanceData.Actor)
		return EStateTreeRunStatus::Failed;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstanceData.Actor);
	if (!ASC)
		return EStateTreeRunStatus::Failed;

	InstanceData.AbilityEndedDelegateHandle = ASC->OnAbilityEnded.AddWeakLambda(
		InstanceData.Actor.Get(),
		[&InstanceData, ExpectedTag = InstanceData.AbilityTag](const FAbilityEndedData& EndedData)
		{
			// 判断结束的 Ability 是不是刚才触发的技能
			if (EndedData.AbilityThatEnded && EndedData.AbilityThatEnded->AbilityTags.HasTag(ExpectedTag))
				InstanceData.bAbilityEnded = true;
		});

	const bool bActivated = ASC->TryActivateAbilitiesByTag(InstanceData.AbilityTag.GetSingleTagContainer());
	if (!bActivated)
	{
		// 如果进入时就在冷却中或被打断无法施法，解绑并立刻判定失败
		ASC->OnAbilityEnded.Remove(InstanceData.AbilityEndedDelegateHandle);
		InstanceData.AbilityEndedDelegateHandle.Reset();
		return EStateTreeRunStatus::Failed;
	}

	InstanceData.bActivatedSuccessfully = true;

	// 3. 核心：返回 Running，告诉 StateTree 任务尚未结束，等待技能放完
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_ActivateAbilityByTag::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	// 4. 每帧轮询回调结果
	if (InstanceData.bAbilityEnded)
	{
		// 技能正常结束或被打断退出，向 StateTree 报告任务完成
		return EStateTreeRunStatus::Succeeded;
	}

	return EStateTreeRunStatus::Running;
}

void FSTT_ActivateAbilityByTag::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	// 5. 状态退出时（无论正常结束还是被外部事件强制打断），必须清理委托句柄
	if (InstanceData.Actor && InstanceData.AbilityEndedDelegateHandle.IsValid())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstanceData.Actor))
		{
			ASC->OnAbilityEnded.Remove(InstanceData.AbilityEndedDelegateHandle);
			InstanceData.AbilityEndedDelegateHandle.Reset();
		}
	}
}
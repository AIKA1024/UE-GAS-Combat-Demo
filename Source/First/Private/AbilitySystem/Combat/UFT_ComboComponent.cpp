#include "AbilitySystem/Combat/UFT_ComboComponent.h"

#include "AbilitySystem/Abilities/FT_ComboGamePlayAbility.h"
#include "AbilitySystem/Combat/UFT_ComboDefinition.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Engine/World.h"
#include "GamePlayTags/FTTag.h"

UFT_ComboComponent::UFT_ComboComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFT_ComboComponent::BeginPlay()
{
	Super::BeginPlay();

	// 监听翻滚 tag 变化：翻滚结束（Roll tag 归零）时触发翻滚中缓冲的攻击
	if (UAbilitySystemComponent* ASC = GetASC())
	{
		ASC->RegisterGameplayTagEvent(FTTag::Abilities::Roll, EGameplayTagEventType::NewOrRemoved)
		   .AddUObject(this, &ThisClass::OnRollTagChanged);
	}
}

void UFT_ComboComponent::RequestAttack(const FGameplayTag& InputTag)
{
	if (!ComboDefinition || !InputTag.IsValid())
		return;

	// 无连段：按输入查起手节点（左→X、右→Y），未配置的输入忽略
	if (CurrentNodeTag.IsValid() == false)
	{
		const FGameplayTag Start = GetStartNodeTag(InputTag);
		if (Start.IsValid())
			ActivateNode(Start);
		return;
	}

	if (bComboAbilityActive)
	{
		// 连段中：组合窗口开 → 按输入跳下一段
		if (bComboWindowOpen)
		{
			const FGameplayTag Next = GetNextNodeTag(CurrentNodeTag, InputTag);
			if (Next.IsValid())
			{
				ClearBufferedAttack();
				ActivateNode(Next);
			}
			// 无跳转则忽略（不推进也不重置）
		}
		else if (bPerInputOpen)
		{
			BufferAttack(InputTag); // 预输入窗口内 → 缓冲这次提前输入
		}
		return;
	}

	// 招式已结束：宽限期内按输入跳下一段；无跳转则按输入回到各自起手（左→X、右→Y）
	if (GetWorld()->GetTimerManager().IsTimerActive(ComboResetTimer))
	{
		// 翻滚中：连段 GA 被 ActivationBlockedTags={Roll} 挡住无法激活，先缓冲，翻滚结束再触发
		if (UAbilitySystemComponent* ASC = GetASC())
		{
			if (ASC->HasMatchingGameplayTag(FTTag::Abilities::Roll))
			{
				BufferAttack(InputTag);
				return;
			}
		}

		const FGameplayTag Next = GetNextNodeTag(CurrentNodeTag, InputTag);
		if (Next.IsValid())
		{
			ClearBufferedAttack();
			ActivateNode(Next);
		}
		else
		{
			const FGameplayTag Start = GetStartNodeTag(InputTag);
			if (Start.IsValid())
				ActivateNode(Start);
		}
	}
	else
	{
		const FGameplayTag Start = GetStartNodeTag(InputTag);
		if (Start.IsValid())
			ActivateNode(Start);
	}
}

void UFT_ComboComponent::NotifyComboActivated(const FGameplayTag& NodeTag)
{
	CurrentNodeTag = NodeTag;
	bComboAbilityActive = true;
	bComboWindowOpen = false; // 新段刚起手，窗口未开
	GetWorld()->GetTimerManager().ClearTimer(ComboResetTimer);
}

void UFT_ComboComponent::NotifyComboEnded(const FGameplayTag& NodeTag, bool bWasCancelled)
{
	// 旧段被新段取消时，新段已调 NotifyComboActivated，这里忽略即可
	if (NodeTag != CurrentNodeTag)
		return;

	bComboAbilityActive = false;
	bComboWindowOpen = false;

	// 先开宽限期再处理缓冲，flush 时 RequestAttack 才能走"宽限期"分支
	if (CurrentNodeTag.IsValid() && !GetWorld()->GetTimerManager().IsTimerActive(ComboResetTimer))
		GetWorld()->GetTimerManager().SetTimer(ComboResetTimer, this, &ThisClass::OnComboReset, ComboResetDelay);

	if (bWasCancelled)
	{
		// 受击等外部打断：丢弃缓冲
		ClearBufferedAttack();
	}
	else
	{
		// 正常收招：触发缓冲的下一段
		FlushBufferedAttack();
	}
}

void UFT_ComboComponent::SetPerInputOpen(bool bOpen)
{
	bPerInputOpen = bOpen;
}

void UFT_ComboComponent::NotifyComboWindowOpened()
{
	// 只有连段中的窗口（bComboAbilityActive=true）才算"可续段窗口"；
	// 翻滚蒙太奇/其它动画的窗口打开时不置位，避免绕过节奏门槛
	bComboWindowOpen = bComboAbilityActive;

	// 窗口打开：触发缓冲的攻击（预输入 / 翻滚中缓冲）
	FlushBufferedAttack();
}

void UFT_ComboComponent::NotifyComboWindowClosed()
{
	bComboWindowOpen = false;
}

void UFT_ComboComponent::BufferAttack(const FGameplayTag& InputTag)
{
	BufferedInputTag = InputTag;
}

void UFT_ComboComponent::ClearBufferedAttack()
{
	BufferedInputTag = FGameplayTag();
}

void UFT_ComboComponent::FlushBufferedAttack()
{
	if (BufferedInputTag.IsValid() == false)
		return;

	const FGameplayTag Input = BufferedInputTag;
	ClearBufferedAttack();
	RequestAttack(Input);
}

void UFT_ComboComponent::OnRollTagChanged(FGameplayTag Tag, int32 NewCount)
{
	// 翻滚结束（Roll tag 归零）→ 触发翻滚中缓冲的攻击
	if (NewCount == 0)
		FlushBufferedAttack();
}

void UFT_ComboComponent::RefreshComboGrace()
{
	if (CurrentNodeTag.IsValid())
		GetWorld()->GetTimerManager().SetTimer(ComboResetTimer, this, &ThisClass::OnComboReset, ComboResetDelay);
}

void UFT_ComboComponent::ResetCombo()
{
	GetWorld()->GetTimerManager().ClearTimer(ComboResetTimer);
	ClearBufferedAttack();
	CurrentNodeTag = FGameplayTag();
	bComboAbilityActive = false;
}

void UFT_ComboComponent::ActivateNode(const FGameplayTag& NodeTag)
{
	if (!ComboDefinition)
		return;

	const FFT_ComboNode* Node = ComboDefinition->FindNode(NodeTag);
	if (!Node || !Node->Ability)
		return;

	UAbilitySystemComponent* ASC = GetASC();
	if (!ASC || ASC->HasMatchingGameplayTag(FTTag::Status::Dead))
		return;

	ASC->TryActivateAbilityByClass(Node->Ability, false);
}

FGameplayTag UFT_ComboComponent::GetNextNodeTag(const FGameplayTag& Current, const FGameplayTag& Input) const
{
	if (ComboDefinition)
	{
		if (const FFT_ComboNode* Node = ComboDefinition->FindNode(Current))
		{
			if (const FGameplayTag* Next = Node->NextByInput.Find(Input))
				return *Next;
		}
	}
	return FGameplayTag();
}

FGameplayTag UFT_ComboComponent::GetStartNodeTag(const FGameplayTag& InputTag) const
{
	if (ComboDefinition)
	{
		// 起手节点完全由 StartNodesByInput 决定（左→X、右→Y 等多起手），未配置的输入返回空
		if (const FGameplayTag* Start = ComboDefinition->StartNodesByInput.Find(InputTag))
		{
			if (Start->IsValid())
				return *Start;
		}
	}
	return FGameplayTag();
}

UAbilitySystemComponent* UFT_ComboComponent::GetASC() const
{
	if (AActor* Owner = GetOwner())
	{
		if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Owner))
			return ASI->GetAbilitySystemComponent();
	}
	return nullptr;
}

void UFT_ComboComponent::OnComboReset()
{
	ClearBufferedAttack();
	CurrentNodeTag = FGameplayTag();
	bComboAbilityActive = false;
}

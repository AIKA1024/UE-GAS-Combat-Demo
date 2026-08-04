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

	// 连段中或宽限期（翻滚后）：一律要求窗口才接下一招；预输入窗口内缓冲
	if (bComboWindowOpen)
	{
		const FGameplayTag Next = GetNextNodeTag(CurrentNodeTag, InputTag);
		if (Next.IsValid())
		{
			ClearBufferedAttack();
			ActivateNode(Next);
		}
		// 无跳转则忽略
	}
	else if (bPerInputOpen)
	{
		BufferAttack(InputTag);
	}
	else
	{
		// 无窗口也无预输入窗口：
		//   连段招播放中（如收招期）→ 忽略，等窗口/预输入
		if (bComboAbilityActive)
			return;

		//   翻滚中 → 忽略（等翻滚窗口/预输入，避免翻滚被重起打断）
		if (UAbilitySystemComponent* ASC = GetASC())
		{
			if (ASC->HasMatchingGameplayTag(FTTag::Abilities::Roll))
				return;
		}

		//   普通招已结束的宽限期 → 立即重置并重新起手（不再等 2s 死输入）
		ResetCombo();
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

	// 开宽限期（ComboResetDelay 秒）：这段内 CurrentNodeTag 保留——翻滚窗口/后续窗口开时能接下一招；
	// 到期 OnComboReset 清空连段。严格模式下宽限期本身不再允许自由接招（必须进窗口）。
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
	// 任何 FT_ComboWindowState 窗口（连段招 / 翻滚蒙太奇）都打开接招门槛，并触发缓冲
	bComboWindowOpen = true;

	// 窗口打开：触发缓冲的攻击（预输入）
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
	// 翻滚结束（Roll tag 归零）→ 丢弃翻滚中残留的缓冲：
	// 预输入的触发交给翻滚窗口（NotifyComboWindowOpened → FlushBufferedAttack），
	// 窗口没触发过的输入不再生效
	if (NewCount == 0)
		ClearBufferedAttack();
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

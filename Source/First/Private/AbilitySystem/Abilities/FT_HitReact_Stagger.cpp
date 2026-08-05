#include "AbilitySystem/Abilities/FT_HitReact_Stagger.h"

#include "Characters/FT_BaseCharacter.h"
#include "GamePlayTags/FTTag.h"

UFT_HitReact_Stagger::UFT_HitReact_Stagger()
{
	FGameplayTagContainer StaggerTags;
	StaggerTags.AddTag(FTTag::Status::HitReact);
	SetAssetTags(StaggerTags);

	// 硬直中不重复触发（同一帧多段伤害只进一次受击）
	ActivationBlockedTags.AddTag(FTTag::Status::HitReact);

	// 打断施法：取消正在播放的攻击 GA
	CancelAbilitiesWithTag.AddTag(FTTag::Status::Attacking);
	// 硬直期间不能起新攻击、不能翻滚
	BlockAbilitiesWithTag.AddTag(FTTag::Status::Attacking);
	BlockAbilitiesWithTag.AddTag(FTTag::Abilities::Roll);

	FAbilityTriggerData TriggerBreak;
	TriggerBreak.TriggerTag = FTTag::Events::Hit::Break;
	TriggerBreak.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerBreak);

	FAbilityTriggerData TriggerNormal;
	TriggerNormal.TriggerTag = FTTag::Events::Hit::Normal;
	TriggerNormal.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerNormal);
}

UAnimMontage* UFT_HitReact_Stagger::GetReactMontage(const AActor* Avatar) const
{
	if (const AFT_BaseCharacter* Character = Cast<const AFT_BaseCharacter>(Avatar))
		return Character->GetStaggerMontage();
	return nullptr;
}

#include "AbilitySystem/Abilities/FT_HitReact_Stagger.h"

#include "Characters/FT_BaseCharacter.h"
#include "GamePlayTags/FTTag.h"

UFT_HitReact_Stagger::UFT_HitReact_Stagger()
{
	FGameplayTagContainer StaggerTags;
	StaggerTags.AddTag(FTTag::Status::HitReact);
	SetAssetTags(StaggerTags);

	// 激活期间把 State.HitReact 挂到 ASC（引擎只同步 ActivationOwnedTags 到角色 tag 容器，
	// SetAssetTags 的 AbilityTags 不进 ASC），FT_BaseCharacter 监听它禁用/恢复移动
	ActivationOwnedTags.AddTag(FTTag::Status::HitReact);

	// 打断施法：取消正在播放的攻击 GA
	CancelAbilitiesWithTag.AddTag(FTTag::Status::Attacking);
	// 硬直期间不能起新攻击、不能翻滚
	BlockAbilitiesWithTag.AddTag(FTTag::Status::Attacking);
	BlockAbilitiesWithTag.AddTag(FTTag::Abilities::Roll);

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

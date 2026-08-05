#include "AbilitySystem/Abilities/FT_HitReact_Armor.h"

#include "Characters/FT_BaseCharacter.h"
#include "GamePlayTags/FTTag.h"

UFT_HitReact_Armor::UFT_HitReact_Armor()
{
	FGameplayTagContainer ArmorHitTags;
	ArmorHitTags.AddTag(FTTag::Status::ArmorHit);
	SetAssetTags(ArmorHitTags);

	// 一帧内多段命中不叠加触发
	ActivationBlockedTags.AddTag(FTTag::Status::ArmorHit);

	// 不 Cancel / 不 Block：霸体受击不打断施法、不封锁动作

	FAbilityTriggerData TriggerArmor;
	TriggerArmor.TriggerTag = FTTag::Events::Hit::Armor;
	TriggerArmor.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerArmor);
}

UAnimMontage* UFT_HitReact_Armor::GetReactMontage(const AActor* Avatar) const
{
	if (const AFT_BaseCharacter* Character = Cast<const AFT_BaseCharacter>(Avatar))
		return Character->GetArmorHitMontage();
	return nullptr;
}

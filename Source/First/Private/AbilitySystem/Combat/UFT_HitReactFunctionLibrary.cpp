#include "AbilitySystem/Combat/UFT_HitReactFunctionLibrary.h"

#include "AbilitySystem/FT_AttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GamePlayTags/FTTag.h"

void UFT_HitReactFunctionLibrary::ProcessHit(const AActor* Instigator, AActor* Target, float PoiseDamage)
{
	if (!Target)
		return;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!ASC || ASC->HasMatchingGameplayTag(FTTag::Status::Dead))
		return;

	const FGameplayAttribute PoiseAttr = UFT_AttributeSet::GetCurrentPoiseAttribute();
	const float CurrentPoise = ASC->GetNumericAttribute(PoiseAttr);

	FGameplayEventData Payload;
	Payload.Instigator = Instigator;
	Payload.Target = Target;

	if (CurrentPoise > 0.f)
	{
		// 霸体中：扣韧性（不重复触发受击硬直）
		const float NewPoise = FMath::Max(0.f, CurrentPoise - FMath::Max(0.f, PoiseDamage));
		ASC->SetNumericAttributeBase(PoiseAttr, NewPoise);

		if (NewPoise > 0.f)
		{
			// 韧性未打空 → 霸体附加受击（不打断施法）
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Target, FTTag::Events::Hit::Armor, Payload);
		}
		else
		{
			// 韧性被这一击打空 → 打断施法 + 完整受击
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Target, FTTag::Events::Hit::Break, Payload);
		}
	}
	else
	{
		// 未处于霸体 → 普通受击
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Target, FTTag::Events::Hit::Normal, Payload);
	}
}

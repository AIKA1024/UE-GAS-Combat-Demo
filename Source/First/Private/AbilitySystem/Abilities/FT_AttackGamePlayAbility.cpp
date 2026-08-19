#include "AbilitySystem/Abilities/FT_AttackGamePlayAbility.h"

#include "AbilitySystem/Combat/UFT_HitReactFunctionLibrary.h"
#include "AbilitySystem/FT_AttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GamePlayTags/FTTag.h"

UFT_AttackGamePlayAbility::UFT_AttackGamePlayAbility()
{
	// 攻击招式：激活期间 ASC 自动携带 State.Attacking，
	// 供韧性判定（ProcessHit）与受击 GA 的 Cancel/BlockAbilitiesWithTag 使用
	FGameplayTagContainer AttackTags;
	AttackTags.AddTag(FTTag::Status::Attacking);
	SetAssetTags(AttackTags);

	// 需要每实例状态（命中事件委托句柄），必须 InstancedPerActor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFT_AttackGamePlayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 新一次出招：清空已结算目标，本招内每个目标只结算一次伤害+受击
	SettledTargets.Reset();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		// 授予本招韧性 + 霸体
		if (PoiseGranted > 0.f)
		{
			ASC->SetNumericAttributeBase(UFT_AttributeSet::GetCurrentPoiseAttribute(), PoiseGranted);
			ASC->AddLooseGameplayTag(FTTag::Status::SuperArmor);
		}

		// 监听攻击命中事件：命中目标后做伤害 + 韧性判定
		FGameplayTagContainer TagFilter;
		TagFilter.AddTag(FTTag::Events::AttackHit);
		AttackHitEventHandle = ASC->AddGameplayEventTagContainerDelegate(
			TagFilter, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::HandleAttackHitEvent));
	}
}

void UFT_AttackGamePlayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		if (PoiseGranted > 0.f)
		{
			// 招式结束/被打断：清空韧性、摘霸体
			ASC->SetNumericAttributeBase(UFT_AttributeSet::GetCurrentPoiseAttribute(), 0.f);
			ASC->RemoveLooseGameplayTag(FTTag::Status::SuperArmor);
		}
		FGameplayTagContainer TagFilter;
		TagFilter.AddTag(FTTag::Events::AttackHit);
		ASC->RemoveGameplayEventTagContainerDelegate(TagFilter, AttackHitEventHandle);
		AttackHitEventHandle.Reset();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UFT_AttackGamePlayAbility::HandleAttackHitEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (!Payload || !EventTag.MatchesTag(FTTag::Events::AttackHit))
		return;

	AActor* Target = const_cast<AActor*>(Payload->Target.Get());
	AActor* Instigator = const_cast<AActor*>(Payload->Instigator.Get());
	if (!Target)
		return;

	// 同一目标本次激活内只结算一次：同帧重复 AttackHit（多组件命中/多 Mesh 命中等）在此合并，
	// 避免伤害与受击事件（ProcessHit）被重复触发
	if (SettledTargets.Contains(Target))
		return;
	SettledTargets.Add(Target);

	// 生命伤害（占位实现：直接扣属性。后续建议替换为正式伤害 GameplayEffect + 减伤/护甲管线）
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target))
	{
		if (Damage > 0.f)
		{
			const FGameplayAttribute HealthAttr = UFT_AttributeSet::GetHealthAttribute();
			const float Health = TargetASC->GetNumericAttribute(HealthAttr);
			TargetASC->SetNumericAttributeBase(HealthAttr, FMath::Max(0.f, Health - Damage));
		}
	}

	// 韧性判定 + 受击事件分流
	UFT_HitReactFunctionLibrary::ProcessHit(Instigator, Target, PoiseDamage);
}

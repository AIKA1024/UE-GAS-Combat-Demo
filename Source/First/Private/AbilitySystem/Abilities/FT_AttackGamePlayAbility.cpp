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

    // 1. 同一目标本次激活内只结算一次
    if (SettledTargets.Contains(Target))
        return;
    SettledTargets.Add(Target);

    UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);

    // 2. 正式伤害 GameplayEffect 结算管线
    if (SourceASC && TargetASC && DamageEffectClass)
    {
        // 从 Source 身上读取攻击力
        bool bFound = false;
        const float AttackPower = SourceASC->GetGameplayAttributeValue(UFT_AttributeSet::GetAttackPowerAttribute(), bFound);
        const float FinalAttackPower = bFound ? AttackPower : 0.0f;

        // 计算最终伤害数值 (AttackPower * EventMagnitude * -1)
        const float EventMagnitude = Payload->EventMagnitude;
        const float FinalDamage = FinalAttackPower * EventMagnitude * -1.0f;

        // 构建 EffectContext（挂载施法者与来源信息）
        FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
        ContextHandle.AddInstigator(Instigator, GetAvatarActorFromActorInfo());

        // 创建 Spec
        const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
            DamageEffectClass,
            GetAbilityLevel(),
            ContextHandle
        );

        if (SpecHandle.IsValid())
        {
            // 通过 SetByCaller 传入伤害 Tag 与计算出的负数数值
            UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
                SpecHandle,
                FTTag::Data::Damage,
                FinalDamage
            );

            // 应用给目标的 ASC
            SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
        }
    }

    // 3. 韧性判定 + 受击事件分流
    UFT_HitReactFunctionLibrary::ProcessHit(Instigator, Target, PoiseDamage);
}

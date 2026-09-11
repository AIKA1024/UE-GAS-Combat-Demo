#include "First/Public/Characters/FT_BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GamePlayTags/FTTag.h"
#include "GameplayAbilitySpec.h"

AFT_BaseCharacter::AFT_BaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* AFT_BaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

void AFT_BaseCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
}

void AFT_BaseCharacter::GiveStartupAbilities()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC)
		return;

	// 启动能力（含受击反应 BP：GA_HitReact_Stagger/Armor）都在角色 BP 的 StartupAbilities 里配
	for (const auto& Ability : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(Ability);
		ASC->GiveAbility(AbilitySpec);
	}

	// 受击硬直期间禁移动：State.HitReact 挂上时 DisableMovement，摘下恢复
	ASC->RegisterGameplayTagEvent(FTTag::Status::HitReact, EGameplayTagEventType::NewOrRemoved)
	   .AddUObject(this, &ThisClass::OnHitReactTagChanged);
}

void AFT_BaseCharacter::OnHitReactTagChanged(FGameplayTag Tag, int32 NewCount)
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (!MoveComp)
		return;

	if (NewCount > 0)
	{
		if (MoveComp->IsFalling())
		{
			// 空中受击：不能 DisableMovement（会切到 MOVE_None，重力/下落模拟全停，人会悬在空中）。
			// 保持 Falling 让重力继续起作用，只清掉水平速度 → 角色原地垂直下落，落地时由引擎自动转 Walking
			MoveComp->Velocity.X = 0.f;
			MoveComp->Velocity.Y = 0.f;
			bHitReactMovementDisabled = false;
		}
		else
		{
			CachedMovementMode = MoveComp->MovementMode;
			MoveComp->DisableMovement();
			bHitReactMovementDisabled = true;
		}
	}
	else if (bHitReactMovementDisabled)
	{
		// 地面硬直结束：恢复受击前的移动模式
		bHitReactMovementDisabled = false;
		if (CachedMovementMode != MOVE_None)
			MoveComp->SetMovementMode(CachedMovementMode);
	}
	// 空中受击：从未 Disable 过移动，无需恢复（Falling/落地逻辑一直在正常运行）
}


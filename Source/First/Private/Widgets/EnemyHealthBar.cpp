// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/EnemyHealthBar.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/FT_AttributeSet.h"
#include "Characters/AFT_BaseEnemy.h"
#include "Components/ActorWidgetComponent.h"
#include "Components/ProgressBar.h"

void UEnemyHealthBar::SetHealthPercent(const float Percent) const
{
	if (HealthBar)
		HealthBar->SetPercent(Percent);
}

void UEnemyHealthBar::InitializeFromOwner()
{
	AActor* Owner = GetOwnerActor();
	if (!Owner)
		return;

	IAbilitySystemInterface* ASI =
		Cast<IAbilitySystemInterface>(Owner);

	if (!ASI)
		return;

	UAbilitySystemComponent* ASC =
		ASI->GetAbilitySystemComponent();

	if (!ASC)
		return;

	InitHealthBar(ASC);
}

void UEnemyHealthBar::InitHealthBar(UAbilitySystemComponent* InASC)
{
	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent.IsValid()) return;

	// 1. 初始化主动拉取一次当前血量
	const float CurrentHealth = AbilitySystemComponent->GetNumericAttribute(UFT_AttributeSet::GetHealthAttribute());
	const float MaxHealth = AbilitySystemComponent->GetNumericAttribute(UFT_AttributeSet::GetMaxHealthAttribute());
	SetHealthPercent(FMath::SafeDivide(CurrentHealth, MaxHealth));

	// 2. 绑定 Health 变化监听（使用 AddWeakLambda 防止悬挂指针）
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UFT_AttributeSet::GetHealthAttribute())
	                      .AddWeakLambda(this, [this](const FOnAttributeChangeData& Data)
	                      {
		                      const float MaxH = AbilitySystemComponent->GetNumericAttribute(
			                      UFT_AttributeSet::GetMaxHealthAttribute());
		                      SetHealthPercent(FMath::SafeDivide(Data.NewValue, MaxH));
	                      });

	// 3. 绑定 MaxHealth 变化监听
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UFT_AttributeSet::GetMaxHealthAttribute())
	                      .AddWeakLambda(this, [this](const FOnAttributeChangeData& Data)
	                      {
		                      const float CurH = AbilitySystemComponent->GetNumericAttribute(
			                      UFT_AttributeSet::GetHealthAttribute());
		                      SetHealthPercent(FMath::SafeDivide(CurH, Data.NewValue));
	                      });
}

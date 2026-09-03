#include "First/Public/Characters/AFT_BaseEnemy.h"

#include "../../../../../../../../UEEngine/UE_5.8/Engine/Plugins/Runtime/GameplayStateTree/Source/GameplayStateTreeModule/Public/Components/StateTreeComponent.h"
#include "AbilitySystem/FT_AbilitySystemComponent.h"
#include "AbilitySystem/FT_AttributeSet.h"
#include "AI/FT_EnemyController.h"
#include "Components/ActorWidgetComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Widgets/EnemyHealthBar.h"


// Sets default values
AFT_BaseEnemy::AFT_BaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UFT_AbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UFT_AttributeSet>("AttributeSet");
	HealthBarWidget = CreateDefaultSubobject<UActorWidgetComponent>("HealthBar");
	StateTreeComp = CreateDefaultSubobject<UStateTreeComponent>("StateTreeComp");
	HealthBarWidget->SetupAttachment(RootComponent);
	AIControllerClass = AFT_EnemyController::StaticClass();
}

void AFT_BaseEnemy::HandleHealthAttributeChange(const FOnAttributeChangeData& Data) const
{
	const float MaxHealth = AbilitySystemComponent->GetNumericAttribute(UFT_AttributeSet::GetMaxHealthAttribute());
	Cast<UEnemyHealthBar>(HealthBarWidget->GetUserWidgetObject())->SetHealthPercent(
		FMath::SafeDivide(Data.NewValue, MaxHealth));
}

void AFT_BaseEnemy::HandleMaxHealthAttributeChange(const FOnAttributeChangeData& Data) const
{
	const float Health = AbilitySystemComponent->GetNumericAttribute(UFT_AttributeSet::GetHealthAttribute());
	Cast<UEnemyHealthBar>(HealthBarWidget->GetUserWidgetObject())->SetHealthPercent(
		FMath::SafeDivide(Health, Data.NewValue));
}

void AFT_BaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

	GiveStartupAbilities();
	
	CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UFT_AttributeSet::GetHealthAttribute()).AddUObject(
	// 	this, &ThisClass::HandleHealthAttributeChange);
	// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UFT_AttributeSet::GetMaxHealthAttribute()).AddUObject(
	// 		this, &ThisClass::HandleMaxHealthAttributeChange);
	// UpdateHealthBarPercent();
}

void AFT_BaseEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HealthBarWidget && CameraManager)
	{
		const FVector WidgetLocation = HealthBarWidget->GetComponentLocation();
		const FVector CameraLocation = CameraManager->GetCameraLocation();

		// 计算朝向摄像机的旋转
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);
		// LookAtRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
		HealthBarWidget->SetWorldRotation(LookAtRotation);
	}
}

UAbilitySystemComponent* AFT_BaseEnemy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFT_BaseEnemy::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = AbilitySystemComponent->GetOwnedGameplayTags();
}

void AFT_BaseEnemy::UpdateHealthBarPercent() const
{
	if (AbilitySystemComponent && HealthBarWidget)
	{
		const float CurrentHealth = AbilitySystemComponent->GetNumericAttribute(UFT_AttributeSet::GetHealthAttribute());
		const float MaxHealth = AbilitySystemComponent->GetNumericAttribute(UFT_AttributeSet::GetMaxHealthAttribute());

		if (const UEnemyHealthBar* HealthBar = Cast<UEnemyHealthBar>(HealthBarWidget->GetUserWidgetObject()))
		{
			HealthBar->SetHealthPercent(FMath::SafeDivide(CurrentHealth, MaxHealth));
		}
	}
}

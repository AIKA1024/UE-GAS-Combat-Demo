
#include "First/Public/Characters/AFT_BaseEnemy.h"

#include "AbilitySystem/FT_AbilitySystemComponent.h"
#include "AbilitySystem/FT_AttributeSet.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"


// Sets default values
AFT_BaseEnemy::AFT_BaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UFT_AbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UFT_AttributeSet>("AttributeSet");
	UAIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("UAIPerceptionComponent");
}

void AFT_BaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetAbilitySystemComponent()->InitAbilityActorInfo(this,this);

	// GAS 不会自动注册挂在 Pawn 上的 AttributeSet（只自动注册 ASC 的子对象），必须显式挂上
	GetAbilitySystemComponent()->AddSpawnedAttribute(AttributeSet);

	GiveStartupAbilities();
}

UAbilitySystemComponent* AFT_BaseEnemy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

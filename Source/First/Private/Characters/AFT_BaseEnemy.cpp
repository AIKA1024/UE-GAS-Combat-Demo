
#include "First/Public/Characters/AFT_BaseEnemy.h"

#include "AbilitySystem/FT_AbilitySystemComponent.h"


// Sets default values
AFT_BaseEnemy::AFT_BaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UFT_AbilitySystemComponent>("AbilitySystemComponent");
}

void AFT_BaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetAbilitySystemComponent()->InitAbilityActorInfo(this,this);
	GiveStartupAbilities();
}

UAbilitySystemComponent* AFT_BaseEnemy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

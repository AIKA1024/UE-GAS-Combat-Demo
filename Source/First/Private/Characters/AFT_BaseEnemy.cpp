#include "First/Public/Characters/AFT_BaseEnemy.h"

#include "AbilitySystem/FT_AbilitySystemComponent.h"
#include "AbilitySystem/FT_AttributeSet.h"
#include "AIController.h"
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
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

	// GAS 不会自动注册挂在 Pawn 上的 AttributeSet（只自动注册 ASC 的子对象），必须显式挂上
	GetAbilitySystemComponent()->AddSpawnedAttribute(AttributeSet);

	GiveStartupAbilities();
}

UAbilitySystemComponent* AFT_BaseEnemy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFT_BaseEnemy::TrackPlayer(AActor* Player, float MaxDistance, float AcceptanceRadius)
{
	if (!IsValid(Player))
		return;
	if (FVector::DistSquared(GetActorLocation(), Player->GetActorLocation()) > FMath::Square(MaxDistance))
	{
		return;
	}

	// 导航需要 AIController：动态生成的敌人可能还没被 Possess，先补一个默认控制器
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
	{
		SpawnDefaultController(); // 返回 void：失败时 Controller 仍为 null
		AIController = Cast<AAIController>(GetController());
	}
	if (AIController == nullptr)
	{
		return;
	}

	// 用 UE 导航（NavMesh 寻路）朝玩家移动，到达 AcceptanceRadius 内停步
	AIController->MoveToActor(Player, AcceptanceRadius, /*bStopOnOverlap*/ true, /*bUsePathfinding*/ true);
}

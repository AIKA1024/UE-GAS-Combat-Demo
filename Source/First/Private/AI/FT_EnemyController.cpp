#include "AI/FT_EnemyController.h"

#include "Kismet/GameplayStatics.h"

AFT_EnemyController::AFT_EnemyController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFT_EnemyController::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsWithTag(GetPawn(),FName("PatrolPoint"),PatrolPoints);
}


void AFT_EnemyController::TrackPlayer(AActor* Player, float MaxDistance, float AcceptanceRadius)
{
	if (!IsValid(Player))
		return;
	if (FVector::DistSquared(GetPawn()->GetActorLocation(), Player->GetActorLocation()) > FMath::Square(MaxDistance))
		return;

	// 用 UE 导航（NavMesh 寻路）朝玩家移动，到达 AcceptanceRadius 内停步
	MoveToActor(Player, AcceptanceRadius, /*bStopOnOverlap*/ true, /*bUsePathfinding*/ true);
	// AIController->MoveToLocation(Player->GetActorLocation(), AcceptanceRadius,  true,  true);
}

void AFT_EnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

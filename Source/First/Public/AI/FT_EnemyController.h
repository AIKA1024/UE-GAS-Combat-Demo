#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FT_EnemyController.generated.h"

UCLASS()
class FIRST_API AFT_EnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AFT_EnemyController();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void TrackPlayer(AActor* Player, float MaxDistance, float AcceptanceRadius = 60.f);

private:
	UPROPERTY(BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	int32 CurrentPatrolIndex = 0;
	
	UPROPERTY(BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	TArray<AActor*> PatrolPoints;
};

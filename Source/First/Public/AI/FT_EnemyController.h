#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "FT_EnemyController.generated.h"

struct FAIStimulus;
class UStateTreeAIComponent;
class UStateTreeComponent;

UCLASS()
class FIRST_API AFT_EnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AFT_EnemyController();
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStateTreeAIComponent> StateTreeComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "First|AI")
	TObjectPtr<AActor> TargetActor = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "First|AI")
	FVector TargetLastLoc;

	virtual FGenericTeamId GetGenericTeamId() const override;

	// 转发态度判定
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
protected:
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void TrackPlayer(AActor* Player, float MaxDistance, float AcceptanceRadius = 60.f);
	
	UFUNCTION(BlueprintCallable)
	void HandelStim(AActor* StimActor,FAIStimulus Stim);

private:
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 CurrentPatrolIndex = 0;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TArray<AActor*> PatrolPoints;
};

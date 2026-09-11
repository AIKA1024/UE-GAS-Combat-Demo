#include "AI/FT_EnemyController.h"

#include "Components/StateTreeAIComponent.h"
#include "GamePlayTags/FTTag.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Prediction.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Touch.h"

AFT_EnemyController::AFT_EnemyController()
{
	PrimaryActorTick.bCanEverTick = true;
	StateTreeComp = CreateDefaultSubobject<UStateTreeAIComponent>("StateTreeComp");
}

void AFT_EnemyController::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsWithTag(GetPawn(), FName("PatrolPoint"), PatrolPoints);
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

void AFT_EnemyController::HandelStim(AActor* StimActor, FAIStimulus Stim)
{
	TargetActor = StimActor;
	TargetLastLoc = StimActor->GetActorLocation();
	
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	5.0f,
	// 	FColor::Red,
	// 	FString::Printf(TEXT("Sense ID: %d"), static_cast<int32>(Stim.Type))
	// );
	
	// --------------------------------------------------------------------------
	// 视觉感知 (Sight)
	// --------------------------------------------------------------------------
	if (Stim.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		if (Stim.WasSuccessfullySensed())
		{
			// 看到目标：进入战斗/追击
			StateTreeComp->SendStateTreeEvent(FTTag::AI::Event::Combat::SightSpotted);
		}
		else
		{
			// 丢失目标：进入最后点搜寻或脱战
			StateTreeComp->SendStateTreeEvent(FTTag::AI::Event::Lost::SightLost);
		}
	}
	// --------------------------------------------------------------------------
	// 听觉感知 (Hearing)
	// --------------------------------------------------------------------------
	else if (Stim.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		// 如果声音带了 Tag（比如开枪是 "Hostile"），可以分流；默认按敌对或可疑派发
		if (Stim.Tag == FName("Hostile"))
		{
			StateTreeComp->SendStateTreeEvent(FTTag::AI::Event::Combat::HostileNoise);
		}
		else
		{
			// 脚步声、杂音等普通声响触发调查
			StateTreeComp->SendStateTreeEvent(FTTag::AI::Event::Investigate::SuspiciousNoise);
		}
	}
	// --------------------------------------------------------------------------
	// 伤害感知 (Damage)
	// --------------------------------------------------------------------------
	else if (Stim.Type == UAISense::GetSenseID<UAISense_Damage>())
	{
		// 受到伤害直接触发战斗反应
		StateTreeComp->SendStateTreeEvent(FTTag::AI::Event::Combat::DamageDirect);
	}
	// --------------------------------------------------------------------------
	// 预测感知 (Prediction)
	// --------------------------------------------------------------------------
	else if (Stim.Type == UAISense::GetSenseID<UAISense_Prediction>())
	{
		// 预测时间到达，前往预判位置排查
		StateTreeComp->SendStateTreeEvent(FTTag::AI::Event::Investigate::PredictionArrival);
	}
	// --------------------------------------------------------------------------
	// 触碰感知 (Touch) —— 可选
	// --------------------------------------------------------------------------
	else if (Stim.Type == UAISense::GetSenseID<UAISense_Touch>())
	{
		// 背后或盲区被撞击直接反击
		StateTreeComp->SendStateTreeEvent(FTTag::AI::Event::Combat::BumpedThreat);
	}
}

void AFT_EnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FGenericTeamId AFT_EnemyController::GetGenericTeamId() const
{
	// 如果 Character 实现了接口，直接返回 Character 的队伍 ID
	if (const IGenericTeamAgentInterface* CharAsTeam = Cast<IGenericTeamAgentInterface>(GetPawn()))
	{
		return CharAsTeam->GetGenericTeamId();
	}
	return FGenericTeamId::NoTeam;
}

ETeamAttitude::Type AFT_EnemyController::GetTeamAttitudeTowards(const AActor& Other) const
{
	// 委托给当前附身的 Character 去做态度判定
	if (const IGenericTeamAgentInterface* CharAsTeam = Cast<IGenericTeamAgentInterface>(GetPawn()))
	{
		return CharAsTeam->GetTeamAttitudeTowards(Other);
	}
	return ETeamAttitude::Neutral;
}

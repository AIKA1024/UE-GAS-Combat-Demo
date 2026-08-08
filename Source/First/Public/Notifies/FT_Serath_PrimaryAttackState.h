// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "FT_Serath_PrimaryAttackState.generated.h"

class AActor;

UCLASS()
class FIRST_API UFT_Serath_PrimaryAttackState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First|Socket")
	float SocketExtensionOffset{0.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First|Socket")
	float SphereTraceRadius{60.f};
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

private:
	TArray<FHitResult> PerformSphereTrace(USkeletalMeshComponent* MeshComp);
	static void SendEventToActors(TArray<FHitResult> Hits, const USkeletalMeshComponent* MeshComp);

	/**
	 * 本次 notify 窗口内已命中的对象（按 MeshComp 区分，多个角色共用同一 notify 资源时互不干扰）。
	 * 纯运行时缓存（弱引用，无需 UPROPERTY 反射/GC 跟踪）。
	 */
	TMap<TWeakObjectPtr<USkeletalMeshComponent>, TArray<TWeakObjectPtr<AActor>>> HitActorsByMesh;

	/** 上一帧的武器 Socket 世界位置（按 MeshComp 区分），用于运动轨迹扫描防高速穿透 */
	TMap<TWeakObjectPtr<USkeletalMeshComponent>, FVector> PrevSocketByMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="First|Debugs", meta=(AllowPrivateAccess=true))
	bool bDrawDebug{false};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	FName SocketName{TEXT("Mace_head")};
};

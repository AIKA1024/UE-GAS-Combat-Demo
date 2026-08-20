// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "FT_NormalAttackState.generated.h"
class AActor;

UCLASS()
class FIRST_API UFT_NormalAttackState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="First|Damage")
	float DamageMultiplier = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="First|Socket")
	float SocketExtensionOffset{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="First|Socket")
	float SphereTraceRadius{60.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="First|Socket")
	FName SocketName{TEXT("SocketName")};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="First|Debugs")
	bool bDrawDebug{false};
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

private:
	TArray<FHitResult> PerformSphereTrace(USkeletalMeshComponent* MeshComp);
	void SendEventToActors(TArray<FHitResult> Hits, const USkeletalMeshComponent* MeshComp) const;

	/**
	 * 本次 notify 窗口内已命中的对象（按 MeshComp 区分，多个角色共用同一 notify 资源时互不干扰）。
	 * 纯运行时缓存（弱引用，无需 UPROPERTY 反射/GC 跟踪）。
	 */
	TMap<TWeakObjectPtr<USkeletalMeshComponent>, TArray<TWeakObjectPtr<AActor>>> HitActorsByMesh;

	/** 上一帧的武器 Socket 世界位置（按 MeshComp 区分），用于运动轨迹扫描防高速穿透 */
	TMap<TWeakObjectPtr<USkeletalMeshComponent>, FVector> PrevSocketByMesh;


};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "FT_Serath_PrimaryAttack.generated.h"

UCLASS()
class FIRST_API UFT_Serath_PrimaryAttack : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First|Socket")
	float SocketExtensionOffset{0.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First|Socket")
	float SphereTraceRadius{60.f};
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference) override;
	
private:
	TArray<FHitResult> PerformSphereTrace(USkeletalMeshComponent* MeshComp) const;
	static void SendEventToActors(TArray<FHitResult> Hits, const USkeletalMeshComponent* MeshComp);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Crash|Debugs", meta=(AllowPrivateAccess=true))
	bool bDrawDebug{false};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	FName SocketName{TEXT("Mace_head")};
};

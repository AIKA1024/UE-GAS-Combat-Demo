// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/FT_Serath_PrimaryAttackState.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "KismetTraceUtils.h"
#include "Characters/Player/FT_PlayerCharacter.h"
#include "GamePlayTags/FTTag.h"

void UFT_Serath_PrimaryAttackState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	const auto HitResults = PerformSphereTrace(MeshComp);
	SendEventToActors(HitResults, MeshComp);
}

TArray<FHitResult> UFT_Serath_PrimaryAttackState::PerformSphereTrace(USkeletalMeshComponent* MeshComp) const
{
	const FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);
	const FVector Start = SocketTransform.GetLocation();
	const FVector ExtendedSocketDirection = SocketTransform.GetRotation().GetForwardVector() * SocketExtensionOffset;
	const FVector End = Start - ExtendedSocketDirection;

	TArray<FHitResult> OutHits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MeshComp->GetOwner());

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);

	UWorld* World = GEngine->GetWorldFromContextObject(MeshComp, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
		return OutHits;
	const bool bHit = World->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_Visibility,
	                                             FCollisionShape::MakeSphere(SphereTraceRadius), Params,
	                                             ResponseParams);

	if (bDrawDebug)
		DrawDebugSphereTraceMulti(World,
		                          Start,
		                          End,
		                          SphereTraceRadius,
		                          EDrawDebugTrace::ForDuration,
		                          bHit,
		                          OutHits,
		                          FColor::Red,
		                          FColor::Green,
		                          5.f);

	return OutHits;
}

void UFT_Serath_PrimaryAttackState::SendEventToActors(TArray<FHitResult> Hits, const USkeletalMeshComponent* MeshComp)
{
	if (!IsValid(MeshComp))
		return;
	for (FHitResult Hit : Hits)
	{
		AFT_PlayerCharacter* PlayerCharacter = Cast<AFT_PlayerCharacter>(Hit.GetActor());
		if (!PlayerCharacter || PlayerCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(FTTag::Status::Dead))
			continue;
		const UAbilitySystemComponent* AbilitySystem = PlayerCharacter->GetAbilitySystemComponent();
		if (!IsValid(AbilitySystem))
			continue;

		FGameplayEffectContextHandle EffectContext = AbilitySystem->MakeEffectContext();
		EffectContext.AddHitResult(Hit);

		FGameplayEventData PayloadData;
		PayloadData.Target = PlayerCharacter;
		PayloadData.ContextHandle = EffectContext;
		PayloadData.Instigator = MeshComp->GetOwner();
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(),
		                                                         FTTag::Events::Player::PrimaryTraceHit,
		                                                         PayloadData);
	}
}

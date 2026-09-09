#include "AbilitySystem/Combat/UFT_HitReactFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GamePlayTags/FTTag.h"

FVector UFT_HitReactFunctionLibrary::ComputeHitDirection(const FVector& InstigatorLocation,
                                                         const FVector& TargetLocation,
                                                         const FRotator& TargetFacing)
{
	FVector ToInstigator = InstigatorLocation - TargetLocation;
	ToInstigator.Z = 0;
	ToInstigator = ToInstigator.GetSafeNormal();
	return TargetFacing.UnrotateVector(ToInstigator);
}

FName UFT_HitReactFunctionLibrary::GetHitReactSectionNameByFVector(const FVector& Vector)
{
	// 输入为目标本地空间的水平受击方向（ComputeHitDirection 的输出）。
	// X 分量决定前/后，Y 分量决定左/右；按主分量把水平面划成 4 个 90° 扇区。
	const float AbsX = FMath::Abs(Vector.X);
	const float AbsY = FMath::Abs(Vector.Y);

	if (AbsX >= AbsY)
	{
		return (Vector.X >= 0.f) ? FName(TEXT("Front")) : FName(TEXT("Back"));
	}

	return (Vector.Y >= 0.f) ? FName(TEXT("Right")) : FName(TEXT("Left"));
}

void UFT_HitReactFunctionLibrary::ProcessHit(const AActor* Instigator, AActor* Target)
{
	if (!Target)
		return;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!ASC || ASC->HasMatchingGameplayTag(FTTag::Status::Dead))
		return;

	FGameplayEventData Payload;
	Payload.Instigator = Instigator;
	Payload.Target = Target;

	// 普通受击
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Target, FTTag::Events::Hit::Normal, Payload);
}

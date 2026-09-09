#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UFT_HitReactFunctionLibrary.generated.h"

/**
 * 受击判定入口：
 *  - 发 Event.Hit.Normal（普通受击），受击 GA 监听该事件激活。
 * 任何伤害来源（攻击通知/技能/远程弹体）统一走这里。
 */
UCLASS()
class FIRST_API UFT_HitReactFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="First|Combat")
	static void ProcessHit(const AActor* Instigator, AActor* Target);

	/**
	 * 计算受击方向：从 Instigator 指向 Target 的向量，旋转到 Target 的本地空间并归一化。
	 * 语义：正前方打来 → (1,0,0)；背后 → (-1,0,0)；左/右侧 → (0,±1,0)。
	 * Instigator 与 Target 重叠时返回零向量。
	 */
	UFUNCTION(BlueprintCallable, Category="First|Combat")
	static FVector ComputeHitDirection(const FVector& InstigatorLocation, const FVector& TargetLocation, const FRotator& TargetFacing);

	/**
	 * 根据目标本地空间的受击方向，返回受击蒙太奇节名：Front / Back / Left / Right。
	 * 输入为 ComputeHitDirection 的输出（X>0 正前方、Y>0 右侧），按主分量划分 4 向。
	 * 零向量（位置重叠无法判定）默认归为 Front。
	 */
	UFUNCTION(BlueprintCallable, Category="First|Combat")
	static FName GetHitReactSectionNameByFVector(const FVector& Vector);
};

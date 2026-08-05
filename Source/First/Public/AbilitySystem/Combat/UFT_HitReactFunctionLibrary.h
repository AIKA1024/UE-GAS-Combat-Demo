#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UFT_HitReactFunctionLibrary.generated.h"

/**
 * 受击判定入口：
 *  - 目标处于霸体（CurrentPoise > 0）→ 扣韧性；韧性未打空发 Event.Hit.Armor（附加受击），打空发 Event.Hit.Break（打断施法 + 完整受击）
 *  - 未处于霸体 → 发 Event.Hit.Normal（普通受击）
 * 受击 GA 各自监听对应事件激活。任何伤害来源（攻击通知/技能/远程弹体）统一走这里。
 */
UCLASS()
class FIRST_API UFT_HitReactFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="First|Combat")
	static void ProcessHit(const AActor* Instigator, AActor* Target, float PoiseDamage);
};

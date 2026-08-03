#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FT_GamePlayAbility.generated.h"

/** 项目通用 GameplayAbility 基类。连段特化逻辑在 UFT_ComboGamePlayAbility。 */
UCLASS()
class FIRST_API UFT_GamePlayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Crash|Debug")
	bool bDrawDebugs = false;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};

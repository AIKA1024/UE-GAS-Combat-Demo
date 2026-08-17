#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "FT_AbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FIRST_API UFT_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnChasingTagChanged(FGameplayTag GameplayTag, int I);
	virtual void BeginPlay() override;
	UFT_AbilitySystemComponent();

protected:
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	
private:
	void HandleAutoActivatedAbility(const FGameplayAbilitySpec& AbilitySpec);
};

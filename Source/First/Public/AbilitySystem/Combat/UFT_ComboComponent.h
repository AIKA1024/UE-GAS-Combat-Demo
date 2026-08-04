#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "UFT_ComboComponent.generated.h"

class UAbilitySystemComponent;
class UFT_ComboDefinition;

/**
 * 连段树驱动：把连段进度作为角色的持久状态（当前节点 tag），与单个招式 GA 的生命周期解耦。
 * 攻击入口按"当前节点 + 输入 tag"在 UFT_ComboDefinition 树里跳下一段；
 * 支持左/右键分支（Primary/Secondary）。翻滚/取消只结束当前招式 GA，不清连段进度。
 * 同时承载预输入缓冲与宽限期（翻滚保留连段）。
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FIRST_API UFT_ComboComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFT_ComboComponent();

	virtual void BeginPlay() override;

	/** 攻击输入入口：按当前节点 + 输入tag 在连段树里跳下一段（Primary/Secondary） */
	UFUNCTION(BlueprintCallable, Category="First|Combo")
	void RequestAttack(const FGameplayTag& InputTag);

	/** 由连段 GA 激活时调用，记录当前节点 */
	void NotifyComboActivated(const FGameplayTag& NodeTag);

	/** 由连段 GA 结束时调用；NodeTag 不是当前节点（被新段取消）时忽略 */
	void NotifyComboEnded(const FGameplayTag& NodeTag, bool bWasCancelled);

	/** 由预输入窗口 notify（UFT_PerInputWindowState）调用，设置/关闭接受按键标志 */
	void SetPerInputOpen(bool bOpen);

	/** 由窗口 notify（UFT_ComboWindowState，连段招或翻滚蒙太奇）调用：打开接招门槛并触发缓冲 */
	void NotifyComboWindowOpened();

	/** 由连段窗口 notify（UFT_ComboWindowState）调用：窗口关闭 */
	void NotifyComboWindowClosed();

	/** 丢弃已缓冲的攻击输入 */
	UFUNCTION(BlueprintCallable, Category="First|Combo")
	void ClearBufferedAttack();

	/** 由翻滚等动作调用，延长连段保留时间 */
	void RefreshComboGrace();

	/** 立即清空连段（受击/死亡/切动作等） */
	UFUNCTION(BlueprintCallable, Category="First|Combo")
	void ResetCombo();

	/** 当前连段节点（空=无连段） */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="First|Combo")
	FGameplayTag CurrentNodeTag;

protected:
	/** 收招后连段保留的宽限时间（秒） */
	UPROPERTY(EditDefaultsOnly, Category="First|Combo")
	float ComboResetDelay = 2.0f;

	/** 连段树定义 */
	UPROPERTY(EditDefaultsOnly, Category="First|Combo")
	TObjectPtr<UFT_ComboDefinition> ComboDefinition;

private:
	void ActivateNode(const FGameplayTag& NodeTag);
	void BufferAttack(const FGameplayTag& InputTag);
	void FlushBufferedAttack();
	FGameplayTag GetNextNodeTag(const FGameplayTag& Current, const FGameplayTag& Input) const;
	FGameplayTag GetStartNodeTag(const FGameplayTag& InputTag) const;
	UAbilitySystemComponent* GetASC() const;
	void OnComboReset();
	void OnRollTagChanged(FGameplayTag Tag, int32 NewCount);

	bool bComboAbilityActive = false;
	bool bComboWindowOpen = false;
	bool bPerInputOpen = false;
	FGameplayTag BufferedInputTag;
	FTimerHandle ComboResetTimer;
};

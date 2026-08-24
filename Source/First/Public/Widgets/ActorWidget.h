// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorWidget.generated.h"

class UActorWidgetComponent;
/**
 * 配合UActorWidgetComponent，允许UActorWidget获得Owner
 */
UCLASS()
class FIRST_API UActorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	AActor* GetOwnerActor() const;

protected:
	TWeakObjectPtr<AActor> OwnerActor;

private:
	friend class UActorWidgetComponent;

	void SetOwnerActor(AActor* InOwnerActor);
	virtual void InitializeFromOwner();
};

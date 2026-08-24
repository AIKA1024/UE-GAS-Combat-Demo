// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ActorWidgetComponent.generated.h"

//允许内部的ActorWidget获得Owner
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FIRST_API UActorWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UActorWidgetComponent();
	virtual void InitWidget() override;
};

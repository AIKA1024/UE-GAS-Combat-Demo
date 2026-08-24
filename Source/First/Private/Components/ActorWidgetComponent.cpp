// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ActorWidgetComponent.h"

#include "Widgets/ActorWidget.h"


// Sets default values for this component's properties
UActorWidgetComponent::UActorWidgetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UActorWidgetComponent::InitWidget()
{
	Super::InitWidget();
	
	if (UActorWidget* TempWidget = Cast<UActorWidget>(GetUserWidgetObject()))
	{
		TempWidget->SetOwnerActor(GetOwner());
		TempWidget->InitializeFromOwner();
	}
}

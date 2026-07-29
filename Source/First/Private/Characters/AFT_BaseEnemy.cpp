// Fill out your copyright notice in the Description page of Project Settings.


#include "First/Public/Characters/AFT_BaseEnemy.h"


// Sets default values
AAFT_BaseEnemy::AAFT_BaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAFT_BaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAFT_BaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAFT_BaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "VS_EnemyCharacter.h"


// Sets default values
AVS_EnemyCharacter::AVS_EnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVS_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVS_EnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AVS_EnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AVS_EnemyCharacter::HandleDeath()
{
	Super::HandleDeath();
	
	// Destroy the enemy actor after a short delay or immediately (assuming immediately for now)
	Destroy();
}


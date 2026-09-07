// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VS_BaseCharacter.h"
#include "VS_EnemyCharacter.generated.h"

UCLASS()
class STADIUM_BLOCKOUT_API AVS_EnemyCharacter : public AVS_BaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVS_EnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void HandleDeath() override;
};

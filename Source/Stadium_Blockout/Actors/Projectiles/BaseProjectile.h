// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/HitResult.h"
#include "BaseProjectile.generated.h"

class USphereComponent;
class UPrimitiveComponent;
class UProjectileMovementComponent;

UCLASS()
class STADIUM_BLOCKOUT_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	FTimerHandle ActivationTimerHandle;
	
	UPROPERTY()
	TObjectPtr<AActor> TargetActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* CollisionComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timing")
	float MinDelayRange = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timing")
	float MaxDelayRange = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float HomingSpeed = 3000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float HomingAcceleration = 20000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float ProjectileDamage = 20.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace Settings")
	float TraceDistance = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace Settings")
	float TraceRadius = 500.f;

	UFUNCTION(BlueprintCallable)
	void ActivateProjectile();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	           FVector NormalImpulse,
	           const FHitResult& Hit);
	
	
	UFUNCTION(BlueprintCallable)
	AActor* FindTargetBySweep();
	
};

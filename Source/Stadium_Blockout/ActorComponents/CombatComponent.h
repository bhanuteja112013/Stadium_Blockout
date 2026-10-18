// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STADIUM_BLOCKOUT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatComponent();
	
	void SetPlayerRef();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combo Settings")
	float ComboInputTimeout = 1.f;

	FTimerHandle ComboResetTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UGameplayEffect> DamageClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat Settings")
	bool bCanAttack = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat Settings")
	bool bIsAttacking = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat Settings")
	int32 ComboCount = 0;
	
	UPROPERTY(BlueprintReadWrite, Category = "Trace Settings")
	FName StartSocket = TEXT("DefaultStartSocket");
	
	UPROPERTY(BlueprintReadWrite, Category = "Trace Settings")
	FName EndSocket = TEXT("DefaultEndSocket");
	
	UPROPERTY(BlueprintReadWrite, Category = "Trace Settings")
	float HalfHeight = 50.f;
	
	UPROPERTY(BlueprintReadWrite, Category = "Trace Settings")
	float Radius = 100.f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Trace Settings")
	float Damage = 10.f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat Settings")
	TObjectPtr<ACharacter> PlayerRef = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat Settings")
	TObjectPtr<UAnimInstance> AnimInstance = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAnimMontage> CurrentAttackMontage;
	
	UPROPERTY()
    TSet<TObjectPtr<AActor>> HitActors;
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartComboAttack();
	
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartSweep();
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void HandleSweepStart();
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void HandleSweepEnd();
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetCapsuleProperties(float CapRadius, float CapHalfHeight);
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetSocketLocations(FName StartSocketName, FName EndSocketName);
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetDamage(float AttackAnimDamage);
	
	UFUNCTION(BlueprintCallable, Category = "Combo")
	void ResetCombo();
};

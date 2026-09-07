// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Weapon.generated.h"

UCLASS()
class STADIUM_BLOCKOUT_API UGA_Weapon : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Weapon();

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Combat")
	void StartSweep();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void ResetMeleeAttack();
	
	UFUNCTION(BlueprintCallable, Category="Combat")
	void ResetCombo();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	TArray<UAnimMontage*> ComboMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float MontagePlayRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Sockets")
	FName StartSocket = "WeaponStart";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Sockets")
	FName EndSocket = "WeaponEnd";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Traces")
	float SweepRadius = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Traces")
	float SweepHalfHeight = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Stats")
	float DamageAmount = 25.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<class UGameplayEffect> DamageEffectClass;

private:
	int32 ComboCount = 0;
	TArray<AActor*> AlreadyHitActors;
	
	UFUNCTION()
	void OnMontageFinished();
};

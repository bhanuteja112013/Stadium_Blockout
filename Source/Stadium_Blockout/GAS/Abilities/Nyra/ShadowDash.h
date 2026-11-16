// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/RootMotionSource.h"
#include "Stadium_Blockout/GAS/Abilities/BaseAbility.h"
#include "ShadowDash.generated.h"

enum class ERootMotionFinishVelocityMode : uint8;
/**
 * 
 */
UCLASS()
class STADIUM_BLOCKOUT_API UShadowDash : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	FName AbilityTaskName = FName("ShadowDash");
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	float Strength = 1500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	float Duration = .3f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	ERootMotionFinishVelocityMode VelocityMode = ERootMotionFinishVelocityMode::ClampVelocity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Settings")
	float ClampVelocityValue = 1200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Settings")
	bool bOnGravity = true;
	
	UFUNCTION()
	FVector CalculateDashDirection();
	
	UFUNCTION()
	void OnFinish();
	
	
};

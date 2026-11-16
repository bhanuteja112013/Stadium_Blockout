// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stadium_Blockout/GAS/Abilities/BaseAbility.h"
#include "Overdrive.generated.h"

/**
 * 
 */
UCLASS()
class STADIUM_BLOCKOUT_API UOverdrive : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overdrive Settings")
	TSubclassOf<UGameplayEffect> EffectToApply;
	
};

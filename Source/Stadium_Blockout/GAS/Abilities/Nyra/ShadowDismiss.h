// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stadium_Blockout/GAS/Abilities/BaseAbility.h"
#include "ShadowDismiss.generated.h"

/**
 * 
 */
UCLASS()
class STADIUM_BLOCKOUT_API UShadowDismiss : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	TSubclassOf<UGameplayEffect> BuffEffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	float DismissDuration = 3.f;
	
	UFUNCTION()
	void OnDismissFinish();
	
	FActiveGameplayEffectHandle SpeedBuffHandle;
	
	


};

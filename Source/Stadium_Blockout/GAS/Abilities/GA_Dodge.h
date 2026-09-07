// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Dodge.generated.h"

UCLASS()
class STADIUM_BLOCKOUT_API UGA_Dodge : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Dodge();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dodge")
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dodge")
	float DodgeForce = 1500.0f;
	
private:
	UFUNCTION()
	void OnMontageFinished();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilityData.generated.h"

/**
 * 
 */
UCLASS()
class STADIUM_BLOCKOUT_API UGameplayAbilityData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Ability Data")
	TSubclassOf<UGameplayAbility> AbilityClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Ability Data")
	int32 AbilityLevel = 1.f;
	
};

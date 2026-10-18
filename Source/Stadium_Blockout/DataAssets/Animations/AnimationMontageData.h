// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AnimationMontageData.generated.h"

UCLASS()
class STADIUM_BLOCKOUT_API UAnimationMontageData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montage Data")
	UAnimMontage* AnimationMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Properties")
	float PlayRate = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Properties")
	float Damage = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Properties")
	bool bIsParryable = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Properties")
	bool bIsBlockable = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Properties")
	float OptimalDistance = 300.f;
};

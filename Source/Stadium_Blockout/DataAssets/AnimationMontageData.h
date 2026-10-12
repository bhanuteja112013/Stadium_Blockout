// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AnimationMontageData.generated.h"

UCLASS()
class STADIUM_BLOCKOUT_API UAnimationMontageData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montage Data")
	UAnimMontage* AnimationMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montage Data")
	float PlayRate = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montage Data")
	bool bIsParryable = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montage Data")
	bool bIsBlockable = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montage Data")
	float OptimalDistance = 300.f;

};

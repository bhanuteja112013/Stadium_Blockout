// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AnimationMontageData.generated.h"

USTRUCT(BlueprintType)
struct FApplyGameplayTags
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montage Data")
	FGameplayTag GameplayTag;
};
UCLASS()
class STADIUM_BLOCKOUT_API UAnimationMontageData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montage Data")
	UAnimMontage* AnimationMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montage Data")
	float PlayRate = 1.f;
	
	TArray<FApplyGameplayTags> ApplyGameplayTags;
};

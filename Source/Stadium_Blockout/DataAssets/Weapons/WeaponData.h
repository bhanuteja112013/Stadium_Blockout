// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Sockets.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "Animation/AnimInstance.h"
#include "Stadium_Blockout/DataAssets/Abilities/GameplayAbilityData.h"
#include "WeaponData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FAnimInstanceDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Instance")
	TObjectPtr<UAnimInstance> AnimInstance = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Instance")
	bool bIsOrientToRotation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Instance")
	bool bIsControllerDesiredRotation = true;
};


UCLASS()
class STADIUM_BLOCKOUT_API UWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	FGameplayTag WeaponTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Data")
	UStaticMesh* WeaponMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	FName AttachSocketName = TEXT("WeaponSocket");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Properties")
	FName TraceToStart = TEXT("DefaultStartSocket");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Properties")
	FName TraceToEnd = TEXT("DefaultEndSocket");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Properties")
	float TraceCapsuleRadius = 15.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	TArray<TObjectPtr<UGameplayAbilityData>> WeaponAbilities; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Properties")
	float TraceCapsuleHalfHeight = 30.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation Properties")
	FAnimInstanceDetails AnimInstanceDetails;
};
	
	

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Stadium_Blockout/DataAssets/Weapons/WeaponData.h"
#include "WeaponManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STADIUM_BLOCKOUT_API UWeaponManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	TArray<FGameplayAbilitySpecHandle> AbilityDataHandles;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ACharacter> CharacterRef;
	
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> EquippedWeaponComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UWeaponData*> AllWeaponsData;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UWeaponData* CurrentEquippedWeaponData = nullptr;
	
	UFUNCTION(BlueprintCallable)
	void EquipWeapon(UWeaponData* NewWeapon);
	
	UFUNCTION(BlueprintCallable)
	void UnequipWeapon();
	
	UFUNCTION(BlueprintCallable)
	void GrantAllWeaponAbilities();
	
	UFUNCTION(BlueprintCallable)
	void RemoveAllWeaponAbilities();
	
	UFUNCTION(BlueprintCallable)
	bool CanSwapWeapon() const;
};

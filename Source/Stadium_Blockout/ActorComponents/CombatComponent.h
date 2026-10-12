// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STADIUM_BLOCKOUT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatComponent();
	
	void SetPlayerRef();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat Settings")
	bool bIsAttacking = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat Settings")
	int32 ComboCount = 0;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat Settings")
	TObjectPtr<ACharacter> PlayerRef = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat Settings")
	TObjectPtr<UAnimInstance> AnimInstance = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAnimMontage> CurrentAttackMontage;
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartComboAttack();
	
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	
	
};

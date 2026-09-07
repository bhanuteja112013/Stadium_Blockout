// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Stadium_Blockout/Interfaces/DamageInterface.h"
#include "AbilitySystemInterface.h"
#include "Components/TimelineComponent.h"
#include "VS_BaseCharacter.generated.h"

class UVS_AttributeSet;
class UAbilitySystemComponent;
struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class STADIUM_BLOCKOUT_API AVS_BaseCharacter : public ACharacter, public IDamageInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVS_BaseCharacter();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* ASC;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UVS_AttributeSet* AttributeSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UStaticMeshComponent* WeaponMesh;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	
	virtual void TakeDamage_Implementation(float Amount, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable, Category="Input Actions")
	void StartJump();
	UFUNCTION(BlueprintCallable, Category="Input Actions")
	void StopJump();
	UFUNCTION(BlueprintCallable, Category="Input Actions")
	void Dodge();
	UFUNCTION(BlueprintCallable, Category="Input Actions")
	virtual void Attack();
	
	UFUNCTION(BlueprintCallable, Category="Combat")
	void NotifyStartSweep();
	
	UFUNCTION(BlueprintCallable, Category="Combat")
	void NotifyResetMeleeAttack();
	
	UFUNCTION(BlueprintCallable, Category="Combat")
	void NotifyComboWindowOpened();
	
	UFUNCTION(BlueprintCallable, Category="Combat")
	void NotifyResetAttacking();
	
	UFUNCTION()
	void OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	virtual void HandleDeath();
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Stats")
	bool bisAttacking = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Stats")
	bool bIsDodging = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AM")
	UAnimMontage* DeathMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AM")
	UAnimMontage* DodgeMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<class UGameplayAbility> WeaponAbilityClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<class UGameplayAbility> DodgeAbilityClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<class UGameplayAbility> JumpAbilityClass;
	
	void GiveDefaultAbilities();
};

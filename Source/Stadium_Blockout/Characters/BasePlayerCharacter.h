// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Stadium_Blockout/ActorComponents/AbilityLoadoutComponent.h"
#include "Stadium_Blockout/ActorComponents/AnimationProfileComponent.h"
#include "Stadium_Blockout/ActorComponents/CombatComponent.h"
#include "Stadium_Blockout/ActorComponents/WeaponManager.h"
#include "Stadium_Blockout/Interfaces/DamageInterface.h"
#include "Stadium_Blockout/GAS//UBR_AttributeSet.h"
#include "BasePlayerCharacter.generated.h"

UCLASS()
class STADIUM_BLOCKOUT_API ABasePlayerCharacter : public ACharacter, public IAbilitySystemInterface, public IDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasePlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage_Implementation(float Amount, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable)
	virtual void HandleDeath_Implementation() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	TObjectPtr<UBR_AttributeSet> AttributeSet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	TObjectPtr<UCombatComponent> CombatComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	TObjectPtr<UWeaponManager> WeaponManager;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	TObjectPtr<UAnimationProfileComponent> AnimationProfileComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	TObjectPtr<UAbilityLoadoutComponent> AbilityLoadoutComponent;
};

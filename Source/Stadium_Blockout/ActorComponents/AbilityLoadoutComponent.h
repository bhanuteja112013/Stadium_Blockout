// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Components/ActorComponent.h"
#include "AbilityLoadoutComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STADIUM_BLOCKOUT_API UAbilityLoadoutComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilityLoadoutComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemRef = nullptr;
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	TMap<FGameplayTag, TSubclassOf<UGameplayAbility>> AbilitiesList;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	TMap<FGameplayTag, FGameplayAbilitySpecHandle> SpecHandles;
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GrantAbility(FGameplayTag AbilityTag);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void RemoveAbility(FGameplayTag AbilityTag);
	
	virtual void OnRegister() override;
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool IsAnyAbilityActive(FGameplayTagContainer AbilityTags);
};


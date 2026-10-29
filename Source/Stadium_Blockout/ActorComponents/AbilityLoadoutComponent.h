// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

public:

	TMap<FGameplayTag, TSubclassOf<FGameplayAbilitySpecHandle>> AbilitySpecHandles;
	
	TMap<FGameplayTag, TSubclassOf<UGameplayAbility>> GainedAbilities;
};

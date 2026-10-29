// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityLoadoutComponent.h"

#include "AbilitySystemComponent.h"


// Sets default values for this component's properties
UAbilityLoadoutComponent::UAbilityLoadoutComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAbilityLoadoutComponent::BeginPlay()
{
	Super::BeginPlay();

	UAbilitySystemComponent* AbilitySystemRef =
		GetOwner()->GetComponentByClass<UAbilitySystemComponent>();

	if (!AbilitySystemRef) return;

	for (const auto& AbilityPair : GainedAbilities)
	{
		
		const FGameplayTag& AbilityTag = AbilityPair.Key;
		TSubclassOf<UGameplayAbility> AbilityClass = AbilityPair.Value;
		
		if (!AbilityClass) continue;
		if (AbilitySystemRef->FindAbilitySpecFromClass(AbilityClass)) continue;

		FGameplayAbilitySpec Spec(AbilityClass, 1);
		AbilitySystemRef->GiveAbility(Spec);
		AbilitySpecHandles.Add(TEXT(pair, Spec.Handle);
	}
}




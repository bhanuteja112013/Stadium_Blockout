// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityLoadoutComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"


// Sets default values for this component's properties
UAbilityLoadoutComponent::UAbilityLoadoutComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UAbilityLoadoutComponent::BeginPlay()
{
	Super::BeginPlay();

	SpecHandles.Empty();
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetOwner());

	if (!ASI)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner does not implement AbilitySystemInterface"));
		return;
	}

	AbilitySystemRef = ASI->GetAbilitySystemComponent();

	UE_LOG(LogTemp, Warning, TEXT("ASC: %s"),
		*GetNameSafe(AbilitySystemRef));
}

void UAbilityLoadoutComponent::GrantAbility(FGameplayTag AbilityTag)
{
	if (!AbilitySystemRef)
	{
		UE_LOG(LogTemp, Error, TEXT("Ability system does not exist"));
		return;
	}
	
	if (SpecHandles.Contains(AbilityTag))
	{
		UE_LOG(LogTemp, Error, TEXT("SpecHandle already contains it! not granting a duplicate"));
		return;
	}

	const TSubclassOf<UGameplayAbility>* AbilityClass = AbilitiesList.Find(AbilityTag);
	if (!AbilityClass || !*AbilityClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Ability class does not exist"));
		return;
	}
	
	FGameplayAbilitySpec Spec(*AbilityClass, 1);
	Spec.GetDynamicSpecSourceTags().AddTag(AbilityTag);
	const FGameplayAbilitySpecHandle Handle = AbilitySystemRef->GiveAbility(Spec);
	
	
	if (!Handle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Ability Spec Handle is not valid"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Ability Successfully granted"));
	SpecHandles.Add(AbilityTag, Handle);
	
}

void UAbilityLoadoutComponent::RemoveAbility(FGameplayTag AbilityTag)
{
	if (!AbilitySystemRef) return;
	if (!SpecHandles.Contains(AbilityTag)) return;
	
	AbilitySystemRef->ClearAbility(SpecHandles.FindRef(AbilityTag));
	SpecHandles.Remove(AbilityTag);
}

void UAbilityLoadoutComponent::OnRegister()
{
	Super::OnRegister();
	
	
	checkf(GetOwner() && GetOwner()->Implements<UAbilitySystemInterface>(), TEXT("Requires AbilitySystemInterface on Owner"));
}

bool UAbilityLoadoutComponent::IsAnyAbilityActive(FGameplayTagContainer AbilityTags)
{
	if (!AbilitySystemRef) return false;
	for (const FGameplayTag& Tag : AbilityTags)
	{
		const FGameplayAbilitySpecHandle* Handle = SpecHandles.Find(Tag);
		
		if (!Handle) continue;
		
		FGameplayAbilitySpec* Spec = AbilitySystemRef->FindAbilitySpecFromHandle(*Handle);
		if (Spec && Spec->IsActive())
		{
			return true;
		}
	}
	return false;
}





// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManager.h"

#include "AbilitySystemComponent.h"
#include "CombatComponent.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
UWeaponManager::UWeaponManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UWeaponManager::BeginPlay()
{
	Super::BeginPlay();

	CharacterRef = Cast<ACharacter>(GetOwner());
	
}


// Called every frame
void UWeaponManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponManager::EquipWeapon(UWeaponData* NewWeapon)
{
	if (!NewWeapon || !NewWeapon->WeaponMesh || !CharacterRef || NewWeapon == CurrentEquippedWeaponData) return;
	if (!CanSwapWeapon()) return;
	
	UnequipWeapon();
	
	CurrentEquippedWeaponData = NewWeapon;
	
	EquippedWeaponComponent = NewObject<UStaticMeshComponent>(CharacterRef);
	
	EquippedWeaponComponent->SetStaticMesh(CurrentEquippedWeaponData->WeaponMesh);
	EquippedWeaponComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	EquippedWeaponComponent->AttachToComponent(CharacterRef->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, CurrentEquippedWeaponData->AttachSocketName);
	EquippedWeaponComponent->RegisterComponent();
	
	if (UCombatComponent* CombatComp = CharacterRef->FindComponentByClass<UCombatComponent>())
	{
		CombatComp->SetSocketLocations(CurrentEquippedWeaponData->TraceToStart, CurrentEquippedWeaponData->TraceToEnd);
		CombatComp->SetCapsuleProperties(CurrentEquippedWeaponData->TraceCapsuleRadius, CurrentEquippedWeaponData->TraceCapsuleHalfHeight);
		CombatComp->bCanAttack = true;
	}
	
	GrantAllWeaponAbilities();
}

void UWeaponManager::UnequipWeapon()
{
	RemoveAllWeaponAbilities();
	if (EquippedWeaponComponent)
	{
		EquippedWeaponComponent->DestroyComponent();
		EquippedWeaponComponent = nullptr;
	}
	
	CurrentEquippedWeaponData = nullptr;
	if (UCombatComponent* CombatComponentRef = CharacterRef->FindComponentByClass<UCombatComponent>())
	{
		CombatComponentRef->bCanAttack = false;
	}
}

void UWeaponManager::GrantAllWeaponAbilities()
{
	if (!CharacterRef) return;
	
	UAbilitySystemComponent* ASC = CharacterRef->GetComponentByClass<UAbilitySystemComponent>();
	if (!ASC) return;
	
	const TArray<TObjectPtr<UGameplayAbilityData>>& Abilities = CurrentEquippedWeaponData->WeaponAbilities;
	
	for (const TObjectPtr<UGameplayAbilityData>& AbilityData : Abilities)
	{
		if (!AbilityData || !AbilityData->AbilityClass) continue;
		FGameplayAbilitySpec AbilitySpec(AbilityData->AbilityClass, AbilityData->AbilityLevel);
		
		FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(AbilitySpec);
		if (Handle.IsValid())
		{
			AbilityDataHandles.Add(Handle);
		}
	}
}

void UWeaponManager::RemoveAllWeaponAbilities()
{
	UAbilitySystemComponent* ASC = CharacterRef
		? CharacterRef->FindComponentByClass<UAbilitySystemComponent>()
		: nullptr;

	if (ASC && ASC->IsOwnerActorAuthoritative())
	{
		for (const FGameplayAbilitySpecHandle& Handle : AbilityDataHandles)
		{
			ASC->ClearAbility(Handle);
		}
	}
	AbilityDataHandles.Empty();
	
}

bool UWeaponManager::CanSwapWeapon() const
{
	if (!CharacterRef) return false;

	UAbilitySystemComponent* ASC =
		CharacterRef->FindComponentByClass<UAbilitySystemComponent>();

	if (!ASC) return false;

	for (const FGameplayAbilitySpecHandle& Handle : AbilityDataHandles)
	{
		const FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(Handle);

		if (Spec && Spec->IsActive())
		{
			return false;
		}
	}
	return true;
}


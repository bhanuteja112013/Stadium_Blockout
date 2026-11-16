// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManager.h"
#include "Stadium_Blockout/DebugHelper.h"
#include "AbilityLoadoutComponent.h"
#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"



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
	
	if (!CharacterRef) return;
	AbilityLoadoutRef = CharacterRef->FindComponentByClass<UAbilityLoadoutComponent>();
	
}


// Called every frame
void UWeaponManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponManager::EquipWeapon(UWeaponData* NewWeapon)
{
	if (!NewWeapon || !NewWeapon->WeaponMesh || !CharacterRef || NewWeapon == CurrentEquippedWeaponData)
	{
		ADebugHelper::Debug("WeaponMesh or Character Ref failed");
		return;
	}
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
	if (CurrentEquippedWeaponData->AnimInstanceDetails.AnimInstance)
	{
		if (!CharacterRef || !CharacterRef->GetCharacterMovement()) return;
		CharacterRef->GetCharacterMovement()->bOrientRotationToMovement = CurrentEquippedWeaponData->AnimInstanceDetails.bIsOrientRotationToMovement;
		CharacterRef->GetCharacterMovement()->bUseControllerDesiredRotation = CurrentEquippedWeaponData->AnimInstanceDetails.bIsControllerDesiredRotation;
		
		if (USkeletalMeshComponent* Mesh = CharacterRef->GetMesh())
		{
			Mesh->SetAnimInstanceClass(CurrentEquippedWeaponData->AnimInstanceDetails.AnimInstance);
		}
	}
	if (CharacterRef)
	{
		GrantAllWeaponAbilities();
	}
}

void UWeaponManager::UnequipWeapon()
{
	if (!CharacterRef) return;
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
	if (!CurrentEquippedWeaponData) return;
	
	UAbilityLoadoutComponent* TempAbilityLoadoutRef = AbilityLoadoutRef = CharacterRef->FindComponentByClass<UAbilityLoadoutComponent>();
	
	if (TempAbilityLoadoutRef)
	{
		const FGameplayTagContainer AbilityTagsContainer = CurrentEquippedWeaponData->WeaponAbilitiesTags;
	
		for (const FGameplayTag& AbilityTag : AbilityTagsContainer)
		{
			TempAbilityLoadoutRef->GrantAbility(AbilityTag);
		}
	}
	else
	{
		ADebugHelper::Debug("LoadoutRef failed");
	}
}

void UWeaponManager::RemoveAllWeaponAbilities()
{
	if (!CurrentEquippedWeaponData) return;
	if (!AbilityLoadoutRef) return;
	
	const FGameplayTagContainer AbilityTagsContainer = CurrentEquippedWeaponData->WeaponAbilitiesTags;
	
	for (const FGameplayTag& AbilityTag : AbilityTagsContainer)
	{
		AbilityLoadoutRef->RemoveAbility(AbilityTag);
	}
}

bool UWeaponManager::CanSwapWeapon() const
{
	if (!CurrentEquippedWeaponData) return true;
	if (!CharacterRef || !AbilityLoadoutRef) return false;
	
	return !AbilityLoadoutRef->IsAnyAbilityActive(CurrentEquippedWeaponData->WeaponAbilitiesTags);
}


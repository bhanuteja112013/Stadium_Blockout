// Fill out your copyright notice in the Description page of Project Settings.


#include "VS_PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
AVS_PlayerCharacter::AVS_PlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	SpringArm->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AVS_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVS_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVS_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
	}
}

void AVS_PlayerCharacter::HandleDeath()
{
	Super::HandleDeath();
	
	if (Controller)
	{
		Controller->UnPossess();
	}
}

void AVS_PlayerCharacter::ToggleLockOn()
{
}

void AVS_PlayerCharacter::ToggleEquipWeapon()
{
	if (!ASC || !HasAuthority()) return;

	if (bWeaponEquipped)
	{
		// Unequip: Find the granted ability by its specific class and remove it
		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(WeaponAbilityClass);
		if (Spec)
		{
			ASC->ClearAbility(Spec->Handle);
		}
		
		bWeaponEquipped = false;
	}
	else
	{
		// Equip: Grant the specific weapon ability
		if (WeaponAbilityClass)
		{
			ASC->GiveAbility(FGameplayAbilitySpec(WeaponAbilityClass, 1, INDEX_NONE, this));
		}
		
		bWeaponEquipped = true;
	}
}

void AVS_PlayerCharacter::Attack()
{
	if (!bWeaponEquipped) return;
	
	Super::Attack();
}

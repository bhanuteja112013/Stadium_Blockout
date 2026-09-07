// Fill out your copyright notice in the Description page of Project Settings.


#include "VS_BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Stadium_Blockout/GAS/VS_AttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Stadium_Blockout/GAS/Abilities/GA_Dodge.h"
#include "Stadium_Blockout/GAS/Abilities/GA_Weapon.h"

AVS_BaseCharacter::AVS_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UVS_AttributeSet>(TEXT("AttributeSet"));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), FName("WeaponSocket")); 
	WeaponMesh->SetVisibility(false); 
}

UAbilitySystemComponent* AVS_BaseCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void AVS_BaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
		GiveDefaultAbilities();
	}
}

void AVS_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AVS_BaseCharacter::TakeDamage_Implementation(float Amount, AActor* DamageCauser)
{
	IDamageInterface::TakeDamage_Implementation(Amount, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("DamageTaken"));
}

void AVS_BaseCharacter::StartJump()
{
	Jump();
}

void AVS_BaseCharacter::StopJump()
{
	StopJumping();
}

void AVS_BaseCharacter::Dodge()
{
    if (bIsDodging && GetCharacterMovement()->IsFalling()) return;

    bIsDodging = true;
	
	if (ASC && DodgeAbilityClass)
	{
		ASC->TryActivateAbilityByClass(DodgeAbilityClass);
	}
	
	
}

void AVS_BaseCharacter::Attack()
{
	if (bisAttacking) return;
	bisAttacking = true;

	if (ASC)
	{
		FGameplayTag AttackTag = FGameplayTag::RequestGameplayTag(FName("Ability.Attack"));
		ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(AttackTag));
	}
}

void AVS_BaseCharacter::HandleDeath()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	GetController()->SetIgnoreLookInput(false);
	GetController()->SetIgnoreMoveInput(true);
	
	//Request Respawn from GameMode
}

void AVS_BaseCharacter::GiveDefaultAbilities()
{
	if (HasAuthority() && ASC)
	{
		if (WeaponAbilityClass)
		{
			ASC->GiveAbility(FGameplayAbilitySpec(WeaponAbilityClass, 1, INDEX_NONE, this));
		}
		if (DodgeAbilityClass)
		{
			ASC->GiveAbility(FGameplayAbilitySpec(DodgeAbilityClass, 1, INDEX_NONE, this));
		}
		if (JumpAbilityClass)
		{
			ASC->GiveAbility(FGameplayAbilitySpec(JumpAbilityClass, 1, INDEX_NONE, this));
		}
	}
}

void AVS_BaseCharacter::NotifyStartSweep()
{
	if (ASC)
	{
		TArray<FGameplayAbilitySpec*> Specs;
		ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Attack"))), Specs);
		for (FGameplayAbilitySpec* Spec : Specs)
		{
			if (Spec->IsActive())
			{
				for (UGameplayAbility* Instance : Spec->GetAbilityInstances())
				{
					if (UGA_Weapon* WeaponAbility = Cast<UGA_Weapon>(Instance))
					{
						WeaponAbility->StartSweep();
					}
				}
			}
		}
	}
}

void AVS_BaseCharacter::NotifyResetMeleeAttack()
{
	if (ASC)
	{
		TArray<FGameplayAbilitySpec*> Specs;
		ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Attack"))), Specs);
		for (FGameplayAbilitySpec* Spec : Specs)
		{
			if (Spec->IsActive())
			{
				for (UGameplayAbility* Instance : Spec->GetAbilityInstances())
				{
					if (UGA_Weapon* WeaponAbility = Cast<UGA_Weapon>(Instance))
					{
						WeaponAbility->ResetMeleeAttack();
					}
				}
			}
		}
	}
}

void AVS_BaseCharacter::NotifyComboWindowOpened()
{
	bisAttacking = false;
}

void AVS_BaseCharacter::NotifyResetAttacking()
{
	bisAttacking = false;
}

void AVS_BaseCharacter::OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsDodging = false;
}

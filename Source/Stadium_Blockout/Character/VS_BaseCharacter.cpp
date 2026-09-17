// Fill out your copyright notice in the Description page of Project Settings.


#include "VS_BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "Stadium_Blockout/GAS/VS_AttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Stadium_Blockout/GAS/Abilities/GA_Dodge.h"
#include "Stadium_Blockout/GAS/Abilities/GA_Weapon.h"

AVS_BaseCharacter::AVS_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), WeaponSocketName); 
	
	
	
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UVS_AttributeSet>(TEXT("AttributeSet"));
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
		GrantAbility(WeaponAbilityClass);
		GrantAbility(DodgeAbilityClass);
		GrantAbility(JumpAbilityClass);
	}
}

void AVS_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	bisAttacking = false;
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
	UE_LOG(LogTemp, Warning, TEXT("[COMBO] Attack() called. bisAttacking=%s"), bisAttacking ? TEXT("true") : TEXT("false"));
	if (bisAttacking) return;
	bisAttacking = true;

	if (ASC && WeaponAbilityClass)
	{
		bool bSuccess = ASC->TryActivateAbilityByClass(WeaponAbilityClass);
		UE_LOG(LogTemp, Warning, TEXT("[COMBO] TryActivateAbilityByClass result: %s"), bSuccess ? TEXT("SUCCESS") : TEXT("FAILED"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[COMBO] ASC=%s, WeaponAbilityClass=%s"), ASC ? TEXT("valid") : TEXT("NULL"), WeaponAbilityClass ? TEXT("valid") : TEXT("NULL"));
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

void AVS_BaseCharacter::GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (HasAuthority() && ASC && AbilityClass)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
	}
}

void AVS_BaseCharacter::RemoveAbility(FGameplayAbilitySpecHandle& Ability)
{
	ASC->ClearAbility(Ability);
}

void AVS_BaseCharacter::ApplyEffectToSelf(
	TSubclassOf<UGameplayEffect> GameplayEffectClass,
	FName DataTag,
	float Magnitude)
{
	FGameplayEffectContextHandle EffectContext =
		GetAbilitySystemComponent()->MakeEffectContext();

	FGameplayEffectSpecHandle GE_Handle =
		GetAbilitySystemComponent()->MakeOutgoingSpec(
			GameplayEffectClass,
			1.0f,
			EffectContext
		);

	if (GE_Handle.IsValid())
	{
		FGameplayEffectSpec* Spec = GE_Handle.Data.Get();

		const FGameplayTag Tag =
			FGameplayTag::RequestGameplayTag(DataTag);

		Spec->SetSetByCallerMagnitude(Tag, Magnitude);

		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*Spec);
	}
}

void AVS_BaseCharacter::NotifyStartSweep()
{
	if (ASC && WeaponAbilityClass)
	{
		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(WeaponAbilityClass);
		UE_LOG(LogTemp, Warning, TEXT("[SWEEP] NotifyStartSweep: Spec=%s, IsActive=%s"),
			Spec ? TEXT("found") : TEXT("NULL"),
			(Spec && Spec->IsActive()) ? TEXT("true") : TEXT("false"));
		if (Spec && Spec->IsActive())
		{
			for (UGameplayAbility* Instance : Spec->GetAbilityInstances())
			{
				if (UGA_Weapon* WeaponAbility = Cast<UGA_Weapon>(Instance))
				{
					UE_LOG(LogTemp, Warning, TEXT("[SWEEP] Calling StartSweep()"));
					WeaponAbility->StartSweep();
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[SWEEP] NotifyStartSweep: ASC=%s, WeaponAbilityClass=%s"),
			ASC ? TEXT("valid") : TEXT("NULL"), WeaponAbilityClass ? TEXT("valid") : TEXT("NULL"));
	}
}

void AVS_BaseCharacter::NotifyResetMeleeAttack()
{
	if (ASC && WeaponAbilityClass)
	{
		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(WeaponAbilityClass);
		if (Spec && Spec->IsActive())
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

void AVS_BaseCharacter::NotifyComboWindowOpened()
{
	UE_LOG(LogTemp, Warning, TEXT("[COMBO] ComboWindowOpened! bisAttacking set to false"));
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

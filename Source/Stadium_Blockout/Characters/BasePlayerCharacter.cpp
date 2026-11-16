// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Stadium_Blockout/GAS/UBR_AttributeSet.h"


// Sets default values
ABasePlayerCharacter::ABasePlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(FName("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UBR_AttributeSet>(FName("AttributeSet"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(FName("CombatComponent"));
	WeaponManager = CreateDefaultSubobject<UWeaponManager>(FName("WeaponManager"));
	AnimationProfileComponent = CreateDefaultSubobject<UAnimationProfileComponent>(FName("AnimationProfileComponent"));
	AbilityLoadoutComponent = CreateDefaultSubobject<UAbilityLoadoutComponent>(FName("AbilityLoadoutComponent"));
}

// Called when the game starts or when spawned
void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	bIsDead = false;
	if (!IsValid(AbilitySystemComponent)) return;
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UBR_AttributeSet::GetMovementSpeedAttribute()).AddUObject(this, &ABasePlayerCharacter::OnMovementSpeedChanged);
	
}

// Called every frame
void ABasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABasePlayerCharacter::TakeDamage_Implementation(float Amount, AActor* DamageCauser)
{
	IDamageInterface::TakeDamage_Implementation(Amount, DamageCauser);
}

void ABasePlayerCharacter::HandleDeath_Implementation()
{
	IDamageInterface::HandleDeath_Implementation();
	bIsDead = true;
}

bool ABasePlayerCharacter::IsTargetLockable_Implementation() const
{
	return bIsLockable && !bIsDead;
}

UAbilitySystemComponent* ABasePlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABasePlayerCharacter::OnMovementSpeedChanged(const FOnAttributeChangeData& Data)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
	}
}


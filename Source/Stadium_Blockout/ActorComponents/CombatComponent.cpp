// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include "AnimationProfileComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimationProfileComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCombatComponent::SetPlayerRef()
{
	PlayerRef = Cast<ACharacter>(GetOwner());
	
	if (PlayerRef && PlayerRef->GetMesh())
	{
		AnimInstance = PlayerRef->GetMesh()->GetAnimInstance();
	}
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetPlayerRef();
}

void UCombatComponent::StartComboAttack()
{
	if (!PlayerRef || PlayerRef->GetCharacterMovement()->IsFalling() || bIsAttacking) return;
	if (!AnimInstance) return;
	
	UAnimationProfileComponent* AnimProfile= PlayerRef->GetComponentByClass<UAnimationProfileComponent>();
	if (!AnimProfile) return;
	
	if (!AnimProfile->CharacterComboAnimData.IsValidIndex(ComboCount))
	{
		ComboCount = 0;
	}
	
	UAnimationMontageData* AnimData = AnimProfile->CharacterComboAnimData[ComboCount];
	if (!AnimData || !AnimData->AnimationMontage) return;
	
	CurrentAttackMontage =AnimData->AnimationMontage;
	const float MontageDuration = AnimInstance->Montage_Play(CurrentAttackMontage, AnimData->PlayRate);
	if (MontageDuration > 0.0f)
	{
		bIsAttacking = true;
			
		++ComboCount;
		AnimInstance->OnMontageEnded.AddUniqueDynamic(this,	&UCombatComponent::OnAttackMontageEnded);
	}
}

void UCombatComponent::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != CurrentAttackMontage) return;
	
	bIsAttacking = false;
	CurrentAttackMontage = nullptr;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include "AbilitySystemComponent.h"
#include "DrawDebugHelpers.h"
#include "AnimationProfileComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Stadium_Blockout/DataAssets/Animations/AnimationMontageData.h"
#include "Stadium_Blockout/Interfaces/DamageInterface.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCombatComponent::SetPlayerRef()
{
	PlayerRef = Cast<ACharacter>(GetOwner());
	if (PlayerRef)
	{
		ASCRef = PlayerRef->FindComponentByClass<UAbilitySystemComponent>();
	}
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
	if (!bCanAttack) return;
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
	SetDamage(AnimData->Damage);;
	const float MontageDuration = AnimInstance->Montage_Play(CurrentAttackMontage, AnimData->PlayRate);
	if (MontageDuration > 0.0f)
	{
		ComboCount++;

		GetWorld()->GetTimerManager().ClearTimer(ComboResetTimer);

		GetWorld()->GetTimerManager().SetTimer(
			ComboResetTimer,
			this,
			&UCombatComponent::ResetCombo,
			ComboInputTimeout,
			false
		);

		AnimInstance->OnMontageEnded.AddUniqueDynamic(this,	&UCombatComponent::OnAttackMontageEnded);
	}
}

void UCombatComponent::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != CurrentAttackMontage) return;
	ComboCount = 0;
	CurrentAttackMontage = nullptr;
}

void UCombatComponent::StartSweep()
{
	if (!PlayerRef || !GetWorld()) return;
	
	TArray<FHitResult> HitResults;
	
	FVector StartLocation = PlayerRef->GetMesh()->GetSocketLocation(StartSocket);
	FVector EndLocation = PlayerRef->GetMesh()->GetSocketLocation(EndSocket);
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerRef);
	
	const FVector BladeDirection = (EndLocation - StartLocation).GetSafeNormal();
	const FQuat SweepRotation = FRotationMatrix::MakeFromZ(BladeDirection).ToQuat();
	
	bool bHit = GetWorld()->SweepMultiByObjectType(HitResults, StartLocation, EndLocation, SweepRotation, ObjectQueryParams, FCollisionShape::MakeCapsule(Radius, HalfHeight), Params);

	FVector SweepCenter = (StartLocation + EndLocation) * 0.5f; 
	FColor DebugColor = bHit ? FColor::Green : FColor::Red; 

	DrawDebugCapsule(
		GetWorld(), 
		SweepCenter, 
		HalfHeight, 
		Radius, 
		SweepRotation, 
		DebugColor, 
		false,
		1.0f,
		0,          
		0.6f        
	);
	
	if (bHit && bIsAttacking)
	{
		for (const FHitResult& HitResult : HitResults)
		{
			if (AActor* HitActor = HitResult.GetActor())
			{
				if (HitActors.Contains(HitActor)) continue;
				if (HitActor->Implements<UDamageInterface>())
				{
					
					float FinalDamage = Damage * DamageMultiplier;
					HitActors.Add(HitActor);
					IDamageInterface::Execute_TakeDamage(HitActor, FinalDamage, GetOwner());

					UE_LOG(LogTemp, Warning, TEXT("Damage Dealt to %s: %f"),
						*HitActor->GetName(), FinalDamage);

				}
			}
		}
	}
	
}

void UCombatComponent::HandleSweepStart()
{
	bIsAttacking = true;
}

void UCombatComponent::HandleSweepEnd()
{
	bIsAttacking = false;
	HitActors.Reset();
}

void UCombatComponent::SetCapsuleProperties(float CapRadius, float CapHalfHeight)
{
	Radius = CapRadius;
	HalfHeight = CapHalfHeight;
}

void UCombatComponent::SetSocketLocations(FName StartSocketName, FName EndSocketName)
{
	StartSocket = StartSocketName;
	EndSocket = EndSocketName;
}

void UCombatComponent::SetDamage(float AttackAnimDamage)
{
	Damage = AttackAnimDamage;
}

void UCombatComponent::ResetCombo()
{
	ComboCount = 0;
}


#include "GA_Weapon.h"
#include "Stadium_Blockout/Character/VS_BaseCharacter.h"
#include "KismetTraceUtils.h"
#include "Engine/World.h"
#include "Stadium_Blockout/Interfaces/DamageInterface.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGA_Weapon::UGA_Weapon()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true; // Crucial for simple combos!
}

void UGA_Weapon::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	if (AVS_BaseCharacter* Char = Cast<AVS_BaseCharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (Char->WeaponMesh) Char->WeaponMesh->SetVisibility(true);
	}
}

void UGA_Weapon::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (AVS_BaseCharacter* Char = Cast<AVS_BaseCharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (Char->WeaponMesh) Char->WeaponMesh->SetVisibility(false);
	}
	Super::OnRemoveAbility(ActorInfo, Spec);
}

void UGA_Weapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (!ComboMontages.IsValidIndex(ComboCount))
	{
		ComboCount = 0;
	}
	
	if (ComboMontages.IsValidIndex(ComboCount))
	{
		UAnimMontage* MontageToPlay = ComboMontages[ComboCount];
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay, MontagePlayRate);
		
		if (PlayMontageTask)
		{
			PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Weapon::OnMontageFinished);
			PlayMontageTask->ReadyForActivation();
		}
		
		ComboCount++;
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UGA_Weapon::OnMontageFinished()
{
	ComboCount = 0;
	if (AVS_BaseCharacter* Char = Cast<AVS_BaseCharacter>(GetAvatarActorFromActorInfo()))
	{
		Char->bisAttacking = false;
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Weapon::ResetCombo()
{
	ComboCount = 0;
}

void UGA_Weapon::StartSweep()
{
	AVS_BaseCharacter* Char = Cast<AVS_BaseCharacter>(GetAvatarActorFromActorInfo());
	if (!Char) return;

	FVector CurrentStart = Char->GetMesh()->GetSocketLocation(StartSocket);
	FVector CurrentEnd = Char->GetMesh()->GetSocketLocation(EndSocket);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Char);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

	FVector SweepDirection = (CurrentEnd - CurrentStart).GetSafeNormal();
	FQuat CapsuleRotation = FRotationMatrix::MakeFromZ(SweepDirection).ToQuat();
	
	TArray<FHitResult> HitResults;
	FCollisionShape SweepShape = FCollisionShape::MakeCapsule(SweepRadius, SweepHalfHeight);

	bool bHit = GetWorld()->SweepMultiByObjectType(HitResults, CurrentStart, CurrentEnd, CapsuleRotation, ObjectParams, SweepShape, QueryParams);
	
	DrawDebugCapsuleTraceSingle(GetWorld(), CurrentStart, CurrentEnd, SweepRadius, SweepHalfHeight, CapsuleRotation.Rotator(), EDrawDebugTrace::ForDuration, bHit, FHitResult(), FLinearColor::Green, FLinearColor::Red, 1.0f);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && !AlreadyHitActors.Contains(HitActor))
			{
				if (HitActor->GetClass()->ImplementsInterface(UDamageInterface::StaticClass()))
				{
					IDamageInterface::Execute_TakeDamage(HitActor, DamageAmount, Char);
				}
				AlreadyHitActors.Add(HitActor);
			}
		}
	}
}

void UGA_Weapon::ResetMeleeAttack()
{
	AlreadyHitActors.Empty();
}

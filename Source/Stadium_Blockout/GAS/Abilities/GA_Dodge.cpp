// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_Dodge.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGA_Dodge::UGA_Dodge()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacter* Char = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Char)
	{
		// 1. Calculate dodge direction (Last input or Forward if standing still)
		FVector DodgeDir = Char->GetLastMovementInputVector();
		if (DodgeDir.IsNearlyZero())
		{
			DodgeDir = Char->GetActorForwardVector();
		}
		
		DodgeDir.Z = 0.0f; // Keep it perfectly horizontal
		DodgeDir.Normalize();

		// 2. Launch Character manually
		Char->LaunchCharacter(DodgeDir * DodgeForce, true, false);
		
		// Optionally rotate the character to face the dodge direction
		Char->SetActorRotation(DodgeDir.Rotation());
	}

	// 3. Play Montage and wait
	if (DodgeMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DodgeMontage, 1.0f);
		if (PlayMontageTask)
		{
			PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Dodge::OnMontageFinished);
			PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Dodge::OnMontageFinished);
			PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Dodge::OnMontageFinished);
			PlayMontageTask->ReadyForActivation();
		}
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UGA_Dodge::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

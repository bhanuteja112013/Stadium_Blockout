// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowDash.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "GameFramework/Character.h"

void UShadowDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	UAbilityTask_ApplyRootMotionConstantForce* RootMotionTask = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
		this,
		AbilityTaskName,
		CalculateDashDirection(),
		Strength,
		Duration,
		false,
		nullptr,
		VelocityMode,
		FVector::ZeroVector,
		ClampVelocityValue,
		bOnGravity);
	if (!RootMotionTask)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
	RootMotionTask->OnFinish.AddDynamic(this, &UShadowDash::OnFinish);
	RootMotionTask->ReadyForActivation();
}

void UShadowDash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FVector UShadowDash::CalculateDashDirection()
{
	ACharacter* CharacterRef = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	
	if (!CharacterRef)
	{
		return FVector::ForwardVector;
	}
	
	if (CharacterRef->IsPlayerControlled())
	{
		FVector Direction = CharacterRef->GetLastMovementInputVector();
		if (!Direction.IsNearlyZero())
		{
			Direction.Z = 0.f;
			return Direction.GetSafeNormal();
		}
	}
	
	FVector Forward = CharacterRef->GetActorForwardVector();
	Forward.Z = 0.f;
	
	return Forward.GetSafeNormal();
	
}

void UShadowDash::OnFinish()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

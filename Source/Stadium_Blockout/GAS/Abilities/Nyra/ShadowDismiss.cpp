// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowDismiss.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

void UShadowDismiss::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	//Cues will take care of animation and visual effects for all abilities
	//Need to properly configure tags as well in BP
	//Only making behavior changes: No Collision and increased movement speed
	
	ACharacter* CharacterRef = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!CharacterRef)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (UCapsuleComponent* CapsuleComponent = CharacterRef->GetCapsuleComponent())
	{
		CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
	BP_ApplyGameplayEffectToOwner(BuffEffectClass, 1.f);
	
	UAbilityTask_WaitDelay* DismissDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, DismissDuration);
	
	if (DismissDelayTask)
	{
		DismissDelayTask->OnFinish.AddDynamic(this, &UShadowDismiss::OnDismissFinish);
		DismissDelayTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
	
}

void UShadowDismiss::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ACharacter* CharacterRef = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		if (UCapsuleComponent* Capsule = CharacterRef->GetCapsuleComponent())
		{
			Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		}
	}
	if (SpeedBuffHandle.IsValid())
	{
		BP_RemoveGameplayEffectFromOwnerWithHandle(SpeedBuffHandle);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UShadowDismiss::OnDismissFinish()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

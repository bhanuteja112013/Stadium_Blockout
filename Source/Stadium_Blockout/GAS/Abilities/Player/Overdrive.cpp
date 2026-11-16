// Fill out your copyright notice in the Description page of Project Settings.


#include "Overdrive.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Stadium_Blockout/ActorComponents/CombatComponent.h"

void UOverdrive::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(GetAvatarActorFromActorInfo());
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectToApply, GetAbilityLevel(), Context);
	
	if (SpecHandle.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	
}

void UOverdrive::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

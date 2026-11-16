// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowProjectile.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Stadium_Blockout/Actors/Projectiles/BaseProjectile.h"

void UShadowProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	ACharacter* CharacterRef = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!CharacterRef || !CharacterRef->GetMesh())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	FVector SpawnLocation = CharacterRef->GetMesh()->GetSocketLocation(SocketName);
	FRotator SpawnRotation = CharacterRef->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = CharacterRef;
	SpawnParams.Instigator = CharacterRef;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	
}

void UShadowProjectile::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

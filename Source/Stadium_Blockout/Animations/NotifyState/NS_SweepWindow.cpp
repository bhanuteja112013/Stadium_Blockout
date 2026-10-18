// Fill out your copyright notice in the Description page of Project Settings.


#include "NS_SweepWindow.h"
#include "Stadium_Blockout/ActorComponents/CombatComponent.h"

void UNS_SweepWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
		
	if (UCombatComponent* CombatComp = GetCombatComponent(MeshComp))
	{
		CombatComp->HandleSweepStart();
	}
	
}

void UNS_SweepWindow::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (UCombatComponent* CombatComp = GetCombatComponent(MeshComp))
	{
		CombatComp->StartSweep();
	}
}

void UNS_SweepWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (UCombatComponent* CombatComp = GetCombatComponent(MeshComp))
	{
		CombatComp->bIsAttacking = false;
		CombatComp->HandleSweepEnd();
	}
}

UCombatComponent* UNS_SweepWindow::GetCombatComponent(USkeletalMeshComponent* MeshComp)
{
	return IsValid(MeshComp) ? MeshComp->GetOwner()->GetComponentByClass<UCombatComponent>() : nullptr;
}
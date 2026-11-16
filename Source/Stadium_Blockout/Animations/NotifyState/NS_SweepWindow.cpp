// Fill out your copyright notice in the Description page of Project Settings.


#include "NS_SweepWindow.h"
#include "Stadium_Blockout/ActorComponents/CombatComponent.h"

void UNS_SweepWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (UCombatComponent* CombatCompRef = GetCombatComponent(MeshComp))
	{
		CombatCompRef->HandleSweepStart();
	}
	
}

void UNS_SweepWindow::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (UCombatComponent* CombatCompRef = GetCombatComponent(MeshComp))
	{
		CombatCompRef->StartSweep();
	}
}

void UNS_SweepWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (UCombatComponent* CombatCompRef = GetCombatComponent(MeshComp))
	{
		CombatCompRef->HandleSweepEnd();
	}
}

UCombatComponent* UNS_SweepWindow::GetCombatComponent(const USkeletalMeshComponent* MeshComp)
{
	if (!MeshComp) return nullptr;
	AActor* Owner = MeshComp->GetOwner();
	
	return Owner ?  MeshComp->GetOwner()->GetComponentByClass<UCombatComponent>() : nullptr;
}
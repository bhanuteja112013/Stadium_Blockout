// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_ComboWindowOpen.h"

#include "Stadium_Blockout/Character/VS_BaseCharacter.h"

void UAN_ComboWindowOpen::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!MeshComp && !MeshComp->GetOwner()) return;
	AVS_BaseCharacter* Char = static_cast<AVS_BaseCharacter*>(MeshComp->GetOwner());
	if (Char)
	{
		Char->NotifyComboWindowOpened();
	}
}

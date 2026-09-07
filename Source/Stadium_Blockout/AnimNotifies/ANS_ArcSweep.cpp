#include "ANS_ArcSweep.h"
#include "Stadium_Blockout/Character/VS_BaseCharacter.h"

void UANS_ArcSweep::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AVS_BaseCharacter* Character = Cast<AVS_BaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->NotifyResetMeleeAttack();
	}
}

void UANS_ArcSweep::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
							   const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	AVS_BaseCharacter* Character = Cast<AVS_BaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->NotifyStartSweep();
	}
	
	
}

void UANS_ArcSweep::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	AVS_BaseCharacter* Character = Cast<AVS_BaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->NotifyResetMeleeAttack();
	}
}

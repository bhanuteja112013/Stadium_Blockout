// Fill out your copyright notice in the Description page of Project Settings.


#include "VS_BasePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Stadium_Blockout/Character/VS_BaseCharacter.h"
#include "Stadium_Blockout/Character/VS_PlayerCharacter.h"

void AVS_BasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		   LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}
void AVS_BasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVS_BasePlayerController::Input_Move);
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVS_BasePlayerController::Input_Look);
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &AVS_BasePlayerController::Input_StartJump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &AVS_BasePlayerController::Input_StopJump);
		EIC->BindAction(DodgeAction, ETriggerEvent::Started, this, &AVS_BasePlayerController::Input_Dodge);
		EIC->BindAction(AttackAction, ETriggerEvent::Started, this, &AVS_BasePlayerController::Input_Attack);
		EIC->BindAction(LockOnAction, ETriggerEvent::Started, this, &AVS_BasePlayerController::Input_LockOn);
		EIC->BindAction(ToggleEquipWeaponAction, ETriggerEvent::Started, this, &AVS_BasePlayerController::Input_ToggleEquip);
	}
}

AVS_PlayerCharacter* AVS_BasePlayerController::GetPossessedCharacter() const
{
	return Cast<AVS_PlayerCharacter>(GetPawn());
}

void AVS_BasePlayerController::Input_Move(const FInputActionValue& Value)
{
	AVS_PlayerCharacter* Char = GetPossessedCharacter();
	if (!Char) return;

	const FVector2D Input = Value.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	Char->AddMovementInput(ForwardDirection, Input.Y);
	Char->AddMovementInput(RightDirection, Input.X);
}

void AVS_BasePlayerController::Input_Look(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();
	AddYawInput(Input.X);
	AddPitchInput(Input.Y);
}

void AVS_BasePlayerController::Input_StartJump()
{
	if (AVS_PlayerCharacter* Char = GetPossessedCharacter())
	{
		Char->Jump();
	}
}

void AVS_BasePlayerController::Input_StopJump()
{
	if (AVS_PlayerCharacter* Char = GetPossessedCharacter())
	{
		Char->StopJumping();
	}
}

void AVS_BasePlayerController::Input_Dodge()
{
	if (AVS_PlayerCharacter* Char = GetPossessedCharacter())
	{
		Char->Dodge();
	}
}

void AVS_BasePlayerController::Input_Attack()
{
	if (AVS_PlayerCharacter* Char = GetPossessedCharacter())
	{
		Char->Attack();
	}
}

void AVS_BasePlayerController::Input_LockOn()
{
	if (AVS_PlayerCharacter* Char = GetPossessedCharacter())
	{
		Char->ToggleLockOn();
	}
}

void AVS_BasePlayerController::Input_ToggleEquip()
{
	if (AVS_PlayerCharacter* Char = GetPossessedCharacter())
	{
		Char->ToggleEquipWeapon();
	}
}

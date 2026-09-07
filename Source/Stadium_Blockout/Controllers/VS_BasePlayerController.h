#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Stadium_Blockout/Character/VS_PlayerCharacter.h"
#include "VS_BasePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class AVS_BaseCharacter;

UCLASS()
class STADIUM_BLOCKOUT_API AVS_BasePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LockOnAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ToggleEquipWeaponAction;

private:

	void Input_Move(const FInputActionValue& Value);
	void Input_Look(const FInputActionValue& Value);
	void Input_StartJump();
	void Input_StopJump();
	void Input_Dodge();
	void Input_Attack();
	void Input_LockOn();
	void Input_ToggleEquip();

	AVS_PlayerCharacter* GetPossessedCharacter() const;
};

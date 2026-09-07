// Fill out your copyright notice in the Description page of Project Settings.


#include "VS_AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Stadium_Blockout/Character/VS_BaseCharacter.h"
#include "Math/UnrealMathUtility.h"

void UVS_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetCurrentHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetCurrentStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

void UVS_AttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth()));

		if (GetCurrentHealth() <= 0.f)
		{
			if (AVS_BaseCharacter* AvatarChar = Cast<AVS_BaseCharacter>(GetOwningActor()))
			{
				AvatarChar->HandleDeath();
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute())
	{
		SetCurrentStamina(FMath::Clamp(GetCurrentStamina(), 0.f, GetMaxStamina()));
	}
}

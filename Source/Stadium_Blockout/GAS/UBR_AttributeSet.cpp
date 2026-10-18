// Fill out your copyright notice in the Description page of Project Settings.


#include "UBR_AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Math/UnrealMathUtility.h"
#include "Stadium_Blockout/Interfaces/DamageInterface.h"

void UBR_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
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

void UBR_AttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth()));

		if (GetCurrentHealth() <= 0.f)
		{
			if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
			{
				if (ASC->GetAvatarActor()->Implements<UDamageInterface>())
				{
					IDamageInterface::Execute_HandleDeath(ASC->GetAvatarActor());
				}
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute())
	{
		SetCurrentStamina(FMath::Clamp(GetCurrentStamina(), 0.f, GetMaxStamina()));
	}
}

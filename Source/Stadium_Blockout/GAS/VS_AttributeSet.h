// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "VS_AttributeSet.generated.h"

/**
 * 
 * 
 */
UCLASS()
class STADIUM_BLOCKOUT_API UVS_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	FGameplayAttributeData CurrentHealth = 100.f;
	ATTRIBUTE_ACCESSORS_BASIC(UVS_AttributeSet, CurrentHealth)
	
	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	FGameplayAttributeData MaxHealth = 100.f;
	ATTRIBUTE_ACCESSORS_BASIC(UVS_AttributeSet, MaxHealth)
	
	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	FGameplayAttributeData CurrentStamina = 100.f;
	ATTRIBUTE_ACCESSORS_BASIC(UVS_AttributeSet, CurrentStamina)
	
	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	FGameplayAttributeData MaxStamina = 100.f;
	ATTRIBUTE_ACCESSORS_BASIC(UVS_AttributeSet, MaxStamina)
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
};

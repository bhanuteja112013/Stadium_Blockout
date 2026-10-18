// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "UBR_AttributeSet.generated.h"

UCLASS()
class STADIUM_BLOCKOUT_API UBR_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	FGameplayAttributeData CurrentHealth = 100.f;
	ATTRIBUTE_ACCESSORS_BASIC(UBR_AttributeSet, CurrentHealth)
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	FGameplayAttributeData MaxHealth = 100.f;
	ATTRIBUTE_ACCESSORS_BASIC(UBR_AttributeSet, MaxHealth)
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	FGameplayAttributeData CurrentStamina = 100.f;
	ATTRIBUTE_ACCESSORS_BASIC(UBR_AttributeSet, CurrentStamina)
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	FGameplayAttributeData MaxStamina = 100.f;
	ATTRIBUTE_ACCESSORS_BASIC(UBR_AttributeSet, MaxStamina)
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
};

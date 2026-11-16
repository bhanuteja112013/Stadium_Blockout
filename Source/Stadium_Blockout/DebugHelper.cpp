// Fill out your copyright notice in the Description page of Project Settings.

#include "Stadium_Blockout/DebugHelper.h"
#include "CoreMinimal.h"


void ADebugHelper::Debug(const FString& Text)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Text);
}

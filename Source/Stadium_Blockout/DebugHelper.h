// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DebugHelper.generated.h"

UCLASS()
class STADIUM_BLOCKOUT_API ADebugHelper : public AActor
{
	GENERATED_BODY()
public:
	static void Debug(const FString& Text);

};

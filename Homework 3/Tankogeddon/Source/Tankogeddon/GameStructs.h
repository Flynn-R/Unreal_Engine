// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GameStructs.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8
{
	Projectile UMETA(DisplayName = "Use projectile"),
	Trace UMETA(DisplayName = "Use trace")
};

UENUM(BlueprintType)
enum class ECannonFireMode : uint8
{
	Single UMETA(DisplayName = "Single mode"),
	Burst UMETA(DisplayName = "Burst mode")
};
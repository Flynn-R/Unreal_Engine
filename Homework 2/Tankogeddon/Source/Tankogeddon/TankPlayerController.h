// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
		class ATankPawn* TankPawn;
	virtual void BeginPlay() override;
	void MoveForward(float value);
	void MoveRight(float value);

public:
	ATankPlayerController() = default;
	virtual void SetupInputComponent() override;
};

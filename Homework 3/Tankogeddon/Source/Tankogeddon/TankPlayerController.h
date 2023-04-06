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
	UPROPERTY()
		FVector mousePos;

protected:
	virtual void BeginPlay() override;
	
	void MoveForward(float value);
	void MoveRight(float value);
	void RotateRight(float value);
	void Fire();
	void FireSpecial();

public:
	ATankPlayerController();
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;
	FVector GetMousePos() { return mousePos; }
};

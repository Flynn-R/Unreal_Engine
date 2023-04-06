// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cannon.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class ACannon;

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	FVector TankMovement(float DeltaTime);
	FRotator TankRotation(float DeltaTime);
	FRotator TurretRotation();
	void SetupCannon();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* CannonSetupPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
		TSubclassOf<ACannon> CannonClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveSpeed = 1000.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSpeed = 500.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float TankInterpolationKey = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float TurretInterpolationKey = 0.5f;

	float _targetForwardAxisValue;
	float _targetRightAxisValue;
	float _targetRotationAxisValue;

	float currentForwardAxisValue;
	float currentRightAxisValue;
	float currentRotationAxisValue;

	UPROPERTY()
		class ATankPlayerController* TankController;
	UPROPERTY()
		ACannon* Cannon;

public:
	ATankPawn();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void MoveForward(float value);
	UFUNCTION()
		void MoveRight(float value);
	UFUNCTION()
		void RotateRight(float value);
	UFUNCTION()
		void Fire();
	UFUNCTION()
		void FireSpecial();
	UFUNCTION()
		void SwitchFireMode();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractPawn.h"
#include "InheritedTankPawn.generated.h"

UCLASS()
class TANKOGEDDON_API AInheritedTankPawn : public AAbstractPawn
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	FVector TankMovement(float DeltaTime);
	FRotator TankRotation(float DeltaTime);
	FRotator TurretRotation();
	UFUNCTION()
		void Die();
	UFUNCTION()
		void DamageTaken(float DamageValue);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
		TSubclassOf<ACannon> SecondaryCannonClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveSpeed = 1000.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSpeed = 500.0f;

	float _targetForwardAxisValue;
	float _targetRightAxisValue;
	float _targetRotationAxisValue;

	float currentForwardAxisValue;
	float currentRightAxisValue;
	float currentRotationAxisValue;

	uint8 SecondaryAmmoLeft;

	UPROPERTY()
		class ATankPlayerController* TankController;

public:
	AInheritedTankPawn();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetupCannon(TSubclassOf<ACannon> newCannon) override;
	ACannon* GetCannon() { return Cannon; }

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
	UFUNCTION()
		void SwitchCannon();
};

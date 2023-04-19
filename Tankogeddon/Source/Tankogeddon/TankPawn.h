// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cannon.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "Components/BoxComponent.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class ACannon;

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn, public IDamageTaker
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	FVector TankMovement(float DeltaTime);
	FRotator TankRotation(float DeltaTime);
	UFUNCTION()
		void Die();
	UFUNCTION()
		void DamageTaken(float DamageValue);

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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UHealthComponent* HealthComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitCollider;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
		TSubclassOf<ACannon> EquippedCannonClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
		TSubclassOf<ACannon> SecondaryCannonClass;
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

	uint8 SecondaryAmmoLeft;

	UPROPERTY()
		class ATankPlayerController* TankController;
	UPROPERTY()
		ACannon* EquippedCannon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", Meta = (MakeEditWidget = true))
		TArray<FVector> PatrollingPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Accuracy")
		float MovementAccuracy = 50.0f;

public:
	FVector GetEyesPosition();

public:
	ATankPawn();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetupCannon(TSubclassOf<ACannon> newCannon);
	ACannon* GetCannon() { return EquippedCannon; }

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
	UFUNCTION()
		void TakeDamage(FDamageData DamageData);
	UFUNCTION()
		TArray<FVector> GetPatrollingPoints() { return PatrollingPoints; }
	UFUNCTION()
		float GetMovementAccuracy() { return MovementAccuracy; }
	UFUNCTION()
		FVector GetTurretForwardVector() { return TurretMesh->GetForwardVector(); }
	UFUNCTION()
		void TurretRotation(FVector MousePos);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "PhysicsProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API APhysicsProjectile : public AProjectile
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UPhysicsComponent* PhysicsComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float MoveAccuracy = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float TrajectorySimulationMaxTime = 50.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float TrajectorySimulationTimeStep = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float TrajectorySimulationSpeed = 20;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		bool bShowTrajectory = false;

	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
		FVector MoveVector;
	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
		TArray<FVector> CurrentTrajectory;
	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
		int32 TrajectoryPointIndex;

public:
	APhysicsProjectile();
	virtual void Start() override;

protected:
	virtual void Move() override;
	/*void Explode();*/
};

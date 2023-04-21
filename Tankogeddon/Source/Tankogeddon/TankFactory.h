// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TargetPoint.h"
#include "TankPawn.h"
#include "TankFactory.generated.h"

UCLASS()
class TANKOGEDDON_API ATankFactory : public AActor, public IDamageTaker
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BuildingMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* TankSpawnPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitCollider;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tank spawn params")
		TSubclassOf<ATankPawn> SpawnTankClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tank spawn params")
		float TankSpawnRate = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tank spawn params")
		TArray<ATargetPoint*> TankWayPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
		class AMapLoader* LinkedMapLoader;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params")
		FName LoadLevelName;

		const FString NewMeshPath = "/Script/Engine.StaticMesh'/Game/StarterContent/Props/SM_Chair.SM_Chair'";
		bool LoadMapIsActive = false;

public:
	ATankFactory();

	UFUNCTION()
		void TakeDamage(FDamageData DamageData) override;

protected:
	virtual void BeginPlay() override;
	void SpawnNewTank();

	UFUNCTION()
		void Die();
	UFUNCTION()
		void DamageTaken(float DamageValue);
	UFUNCTION()
		void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

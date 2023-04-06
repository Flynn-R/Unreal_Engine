// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs.h"
#include "Cannon.generated.h"

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	void Reload();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Component")
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Component")
		class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon params")
		float FireRate = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon params")
		float FireRange = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon params")
		float FireDamage = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon params")
		ECannonType Type = ECannonType::Projectile;

	FTimerHandle ReloadTimerHandle;
	bool ReadyToFire = false;

public:
	ACannon();

	void Fire();
	void FireSpecial();
	bool IsReadyToFire();
};

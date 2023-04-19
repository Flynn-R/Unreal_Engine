// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs.h"
#include <vector>
#include "Camera/CameraShakeBase.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "Particles/ParticleSystemComponent.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon params")
		uint8 Ammo = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon params")
		ECannonFireMode Mode = ECannonFireMode::Single;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon params")
		uint8 BurstFireShots = 3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon params")
		float BurstFireShotsInterval = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon params")
		float BurstFireRate = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon params")
		TSubclassOf<class AProjectile> ProjectileClass;

	FTimerHandle ReloadTimerHandle;
	std::vector<FTimerHandle> BurstTimerHandle;
	bool bReadyToFire = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		UParticleSystemComponent* ShootEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		UAudioComponent* AudioEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UForceFeedbackEffect* ShootForceEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		TSubclassOf<UCameraShakeBase> ShootShake;

public:
	ACannon();

	void Fire();
	void FireSpecial();
	void FireSingle();
	void FireBurst();
	void SwitchFireMode();
	bool IsReadyToFire();
	void SetAmmo(uint8 AmmoCapacity);
	uint8 GetAmmo() { return Ammo; }
};

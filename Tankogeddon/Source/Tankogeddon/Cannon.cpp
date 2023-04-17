// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Projectile.h"
#include "DamageTaker.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = scene;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot effect"));
	ShootEffect->SetupAttachment(ProjectileSpawnPoint);
	ShootEffect->SetAutoActivate(false);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio effect"));
	AudioEffect->SetupAttachment(ProjectileSpawnPoint);
	AudioEffect->SetAutoActivate(false);
}

void ACannon::FireSingle()
{
	if (Type == ECannonType::Projectile)
	{
		GEngine->AddOnScreenDebugMessage(0, 0.5, FColor::Red, "Fire: projectile");

		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,
			ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation());
		if (projectile)
			projectile->Start();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 0.5, FColor::Yellow, "Fire: trace");

		FHitResult hitResult;
		FCollisionQueryParams traceParams =
			FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		traceParams.bTraceComplex = true;
		traceParams.bReturnPhysicalMaterial = false;
		FVector start = ProjectileSpawnPoint->GetComponentLocation();
		FVector end = ProjectileSpawnPoint->GetForwardVector() * FireRange + start;
		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end,
			ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Red, false,
				0.5f, 0, 5);
			if (hitResult.GetActor())
			{
				IDamageTaker* DamageTakerActor = Cast<IDamageTaker>(hitResult.GetActor());
				if (DamageTakerActor)
				{
					FDamageData DamageData;
					DamageData.DamageValue = FireDamage;
					DamageData.Instigator = this;
					DamageData.DamageMaker = this;

					DamageTakerActor->TakeDamage(DamageData);
				}
				else
					hitResult.GetActor()->Destroy();
			}
		}
		else
			DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.5f, 0, 5);
	}

	GEngine->AddOnScreenDebugMessage(1, 1, FColor::White, FString::Printf(TEXT("Ammo left: %d"), Ammo));
}

void ACannon::Fire()
{
	if (!bReadyToFire || !(Ammo > 0))
		return;
	
	--Ammo;
	bReadyToFire = false;

	ShootEffect->ActivateSystem();
	AudioEffect->Play();

	if (GetOwner() && GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (ShootForceEffect)
		{
			FForceFeedbackParameters shootForceEffectParams;
			shootForceEffectParams.bLooping = false;
			shootForceEffectParams.Tag = "shootForceEffectParams";
			GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ShootForceEffect, shootForceEffectParams);
		}

		if (ShootShake)
			GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(ShootShake);
	}

	if (Mode == ECannonFireMode::Single)
	{
		FireSingle();
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
	}
	else
	{
		FireBurst();
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, BurstFireShots / BurstFireRate, false);
		BurstTimerHandle.clear();
	}
}

void ACannon::FireSpecial()
{
	if (!bReadyToFire || !(Ammo > 0))
		return;

	--Ammo;
	bReadyToFire = false;
	if (Type == ECannonType::Projectile)
		GEngine->AddOnScreenDebugMessage(2, 2, FColor::Cyan, "Fire: armor piercing projectile");
	else
		GEngine->AddOnScreenDebugMessage(2, 2, FColor::Blue, "Fire: high frequency beam trace");

	GEngine->AddOnScreenDebugMessage(3, 2, FColor::White, FString::Printf(TEXT("Ammo left: %d"), Ammo));
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 3 / FireRate, false);
}

void ACannon::FireBurst()
{
	for (auto i = 0; i < BurstFireShots; ++i)
	{
		BurstTimerHandle.push_back(FTimerHandle());
		GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle.back(), this, &ACannon::FireSingle, 1 / FireRate, false, i * BurstFireShotsInterval);
	}
}

void ACannon::SwitchFireMode()
{
	if (Mode == ECannonFireMode::Single)
		Mode = ECannonFireMode::Burst;
	else
		Mode = ECannonFireMode::Single;
}

bool ACannon::IsReadyToFire()
{
	return bReadyToFire;
}

void ACannon::SetAmmo(uint8 AmmoCapacity)
{
	if (AmmoCapacity <= 255)
		Ammo = AmmoCapacity;
	else
		Ammo = 255;
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}

void ACannon::Reload()
{
	bReadyToFire = true;
}

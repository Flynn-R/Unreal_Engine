// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = scene;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void ACannon::FireSingle()
{
	if (Type == ECannonType::Projectile)
		GEngine->AddOnScreenDebugMessage(0, 0.5, FColor::Red, "Fire: projectile");
	else
		GEngine->AddOnScreenDebugMessage(0, 0.5, FColor::Yellow, "Fire: trace");

	GEngine->AddOnScreenDebugMessage(1, 1, FColor::White, FString::Printf(TEXT("Ammo left: %d"), Ammo));
}

void ACannon::Fire()
{
	if (!bReadyToFire || !(Ammo > 0))
		return;
	
	--Ammo;
	bReadyToFire = false;

	if (Mode == ECannonFireMode::Single)
	{
		FireSingle();
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
	}
	else
	{
		FireBurst();
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, Shots / BurstFireRate, false);
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
	for (auto i = 0; i < Shots; ++i)
	{
		BurstTimerHandle.push_back(FTimerHandle());
		GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle.back(), this, &ACannon::FireSingle, 1 / FireRate, false, i * ShotsInterval);
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

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}

void ACannon::Reload()
{
	bReadyToFire = true;
}

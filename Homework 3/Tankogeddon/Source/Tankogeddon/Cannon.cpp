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

void ACannon::Fire()
{
	if (!ReadyToFire)
		return;

	ReadyToFire = false;
	if (Type == ECannonType::Projectile)
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, "Fire: projectile");
	else
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Yellow, "Fire: trace");

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}

void ACannon::FireSpecial()
{
	if (!ReadyToFire)
		return;

	ReadyToFire = false;
	if (Type == ECannonType::Projectile)
		GEngine->AddOnScreenDebugMessage(1, 1, FColor::Cyan, "Fire: armor piercing projectile");
	else
		GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, "Fire: high frequency beam trace");

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 2 / FireRate, false);
}

bool ACannon::IsReadyToFire()
{
	return ReadyToFire;
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}

void ACannon::Reload()
{
	ReadyToFire = true;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MapLoader.h"

ATankFactory::ATankFactory()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComponent;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building mesh"));
	BuildingMesh->SetupAttachment(sceneComponent);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Tank spawn point"));
	TankSpawnPoint->AttachToComponent(sceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(sceneComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDamaged.AddUObject(this, &ATankFactory::DamageTaken);
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);

	HitCollider->OnComponentBeginOverlap.AddDynamic(this, &ATankFactory::OnMeshOverlapBegin);
}

void ATankFactory::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankFactory::BeginPlay()
{
	Super::BeginPlay();

	if (LinkedMapLoader)
		LinkedMapLoader->SetIsActivated(false);
	
	FTimerHandle SpawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ATankFactory::SpawnNewTank, TankSpawnRate, true, TankSpawnRate);
}

void ATankFactory::SpawnNewTank()
{
	FTransform spawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1));
	ATankPawn* newTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, spawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	newTank->SetPatrollingPoints(TankWayPoints);
	UGameplayStatics::FinishSpawningActor(newTank, spawnTransform);
}

void ATankFactory::Die()
{
	LoadMapIsActive = true;

	UStaticMesh* mesh = LoadObject<UStaticMesh>(this, *NewMeshPath);
	if (mesh)
		BuildingMesh->SetStaticMesh(mesh);

	if (LinkedMapLoader)
		LinkedMapLoader->SetIsActivated(true);
	/*Destroy();*/
}

void ATankFactory::DamageTaken(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Factory %s took damage: %f. Health: %f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

void ATankFactory::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!LoadMapIsActive)
		return;

	ATankPawn* playerPawn = Cast<ATankPawn>(OtherActor);
	if (playerPawn)
		UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);
}

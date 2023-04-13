// Fill out your copyright notice in the Description page of Project Settings.


#include "InheritedTurret.h"

AInheritedTurret::AInheritedTurret()
{
	HealthComponent->OnDie.AddUObject(this, &AInheritedTurret::Die);
	HealthComponent->OnDamaged.AddUObject(this, &AInheritedTurret::DamageTaken);

	UStaticMesh* TurretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if (TurretMeshTemp)
		TurretMesh->SetStaticMesh(TurretMeshTemp);

	UStaticMesh* BodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if (BodyMeshTemp)
		BodyMesh->SetStaticMesh(BodyMeshTemp);
}

void AInheritedTurret::Die()
{
	Destroy();
}

void AInheritedTurret::DamageTaken(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Turret %s took damage: %f. Health: %f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

void AInheritedTurret::BeginPlay()
{
	Super::BeginPlay();

	SetupCannon(CannonClass);
	Cannon->SetAmmo(255);

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FTimerHandle TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &AInheritedTurret::Targeting, TargetingRate, true, TargetingRate);
}

void AInheritedTurret::Destroyed()
{
	if (Cannon)
		Cannon->Destroy();
}

void AInheritedTurret::Targeting()
{
	if (IsPlayerInRange())
		RotateToPlayer();
	if (CanFire() && Cannon && Cannon->IsReadyToFire())
		Fire();
}

void AInheritedTurret::RotateToPlayer()
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(CurrentRotation, TargetRotation, TargetingSpeed));
}

bool AInheritedTurret::IsPlayerInRange()
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetRange;
}

bool AInheritedTurret::CanFire()
{
	FVector TargetingDirection = TurretMesh->GetForwardVector();
	FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	DirectionToPlayer.Normalize();
	float AimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(TargetingDirection, DirectionToPlayer)));
	return AimAngle <= Accuracy;
}

void AInheritedTurret::Fire()
{
	if (Cannon)
		Cannon->Fire();
}

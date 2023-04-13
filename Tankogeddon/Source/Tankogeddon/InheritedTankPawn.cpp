// Fill out your copyright notice in the Description page of Project Settings.


#include "InheritedTankPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"

AInheritedTankPawn::AInheritedTankPawn() : AAbstractPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	HealthComponent->OnDie.AddUObject(this, &AInheritedTankPawn::Die);
	HealthComponent->OnDamaged.AddUObject(this, &AInheritedTankPawn::DamageTaken);

	HitCollider->SetupAttachment(BodyMesh);
}

void AInheritedTankPawn::BeginPlay()
{
	Super::BeginPlay();

	TankController = Cast<ATankPlayerController>(GetController());
	SetupCannon(CannonClass);
}

FVector AInheritedTankPawn::TankMovement(float DeltaTime)
{
	return FVector(GetActorLocation() + (GetActorForwardVector() * currentForwardAxisValue + GetActorRightVector() * currentRightAxisValue) * MoveSpeed * DeltaTime);
}

FRotator AInheritedTankPawn::TankRotation(float DeltaTime)
{
	return FRotator(0, GetActorRotation().Yaw + RotationSpeed * currentRotationAxisValue * DeltaTime, 0);
}

FRotator AInheritedTankPawn::TurretRotation()
{
	FRotator currentRotation = TurretMesh->GetComponentRotation();
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TankController->GetMousePos());
	targetRotation.Pitch = currentRotation.Pitch;
	targetRotation.Roll = currentRotation.Roll;
	return FRotator(FMath::Lerp(currentRotation, targetRotation, TargetingSpeed));
}

void AInheritedTankPawn::Die()
{
	Destroy();
}

void AInheritedTankPawn::DamageTaken(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Tank %s took damage: %f. Health: %f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

void AInheritedTankPawn::SetupCannon(TSubclassOf<ACannon> newCannon)
{
	if (!SecondaryCannonClass)
		SecondaryCannonClass = CannonClass;

	if (Cannon)
	{
		SecondaryAmmoLeft = Cannon->GetAmmo();
		Cannon->Destroy();
	}

	CannonClass = newCannon;

	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AInheritedTankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentForwardAxisValue = FMath::Lerp(currentForwardAxisValue, _targetForwardAxisValue, TargetingSpeed);
	currentRightAxisValue = FMath::Lerp(currentRightAxisValue, _targetRightAxisValue, TargetingSpeed);
	currentRotationAxisValue = FMath::Lerp(currentRotationAxisValue, _targetRotationAxisValue, TargetingSpeed);

	SetActorLocation(TankMovement(DeltaTime), true);
	SetActorRotation(TankRotation(DeltaTime));
	if (TankController)
		TurretMesh->SetWorldRotation(TurretRotation());
}

void AInheritedTankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AInheritedTankPawn::MoveForward(float value)
{
	_targetForwardAxisValue = value;
}

void AInheritedTankPawn::MoveRight(float value)
{
	_targetRightAxisValue = value;
}

void AInheritedTankPawn::RotateRight(float value)
{
	_targetRotationAxisValue = value;
}

void AInheritedTankPawn::Fire()
{
	if (Cannon)
		Cannon->Fire();
}

void AInheritedTankPawn::FireSpecial()
{
	if (Cannon)
		Cannon->FireSpecial();
}

void AInheritedTankPawn::SwitchFireMode()
{
	if (Cannon)
		Cannon->SwitchFireMode();
}

void AInheritedTankPawn::SwitchCannon()
{
	if (!SecondaryCannonClass)
		return;

	auto ammo = SecondaryAmmoLeft;
	auto temp = CannonClass;
	CannonClass = SecondaryCannonClass;
	SecondaryCannonClass = temp;
	SetupCannon(CannonClass);
	Cannon->SetAmmo(ammo);
}

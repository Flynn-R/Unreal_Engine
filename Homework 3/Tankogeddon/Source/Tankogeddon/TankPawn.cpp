// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"

ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	TankController = Cast<ATankPlayerController>(GetController());
	SetupCannon();
}

FVector ATankPawn::TankMovement(float DeltaTime)
{
	return FVector(GetActorLocation() + (GetActorForwardVector() * currentForwardAxisValue + GetActorRightVector() * currentRightAxisValue) * MoveSpeed * DeltaTime);
}

FRotator ATankPawn::TankRotation(float DeltaTime)
{
	return FRotator(0, GetActorRotation().Yaw + RotationSpeed * currentRotationAxisValue * DeltaTime, 0);
}

FRotator ATankPawn::TurretRotation()
{
	FRotator currentRotation = TurretMesh->GetComponentRotation();
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TankController->GetMousePos());
	targetRotation.Pitch = currentRotation.Pitch;
	targetRotation.Roll = currentRotation.Roll;
	return FRotator(FMath::Lerp(currentRotation, targetRotation, TurretInterpolationKey));
}

void ATankPawn::SetupCannon()
{
	if (Cannon)
		Cannon->Destroy();

	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentForwardAxisValue = FMath::Lerp(currentForwardAxisValue, _targetForwardAxisValue, TankInterpolationKey);
	currentRightAxisValue = FMath::Lerp(currentRightAxisValue, _targetRightAxisValue, TankInterpolationKey);
	currentRotationAxisValue = FMath::Lerp(currentRotationAxisValue, _targetRotationAxisValue, TankInterpolationKey);

	SetActorLocation(TankMovement(DeltaTime), true);
	SetActorRotation(TankRotation(DeltaTime));
	if (TankController)
		TurretMesh->SetWorldRotation(TurretRotation());
}

void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATankPawn::MoveForward(float value)
{
	_targetForwardAxisValue = value;
}

void ATankPawn::MoveRight(float value)
{
	_targetRightAxisValue = value;
}

void ATankPawn::RotateRight(float value)
{
	_targetRotationAxisValue = value;
}

void ATankPawn::Fire()
{
	if (Cannon)
		Cannon->Fire();
}

void ATankPawn::FireSpecial()
{
	if (Cannon)
		Cannon->FireSpecial();
}

void ATankPawn::SwitchFireMode()
{
	if (Cannon)
		Cannon->SwitchFireMode();
}
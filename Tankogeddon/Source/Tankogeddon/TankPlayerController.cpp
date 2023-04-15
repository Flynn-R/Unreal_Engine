// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"

ATankPlayerController::ATankPlayerController()
{
	bShowMouseCursor = true;
}

void ATankPlayerController::Tick(float DeltaTime)
{
	FVector pawnPos;
	DeprojectMousePositionToWorld(mousePos, pawnPos);
	pawnPos = TankPawn->GetActorLocation();
	mousePos.Z = pawnPos.Z;
	FVector dir = mousePos - pawnPos;
	dir.Normalize();
	mousePos = pawnPos + dir * 1000;
	/*DrawDebugLine(GetWorld(), pawnPos, mousePos, FColor::Green, false, 0.1f, 0, 5);*/
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
	/*TankPawn = Cast<AInheritedTankPawn>(GetPawn());*/
}

void ATankPlayerController::MoveForward(float value)
{
	TankPawn->MoveForward(value);
}

void ATankPlayerController::MoveRight(float value)
{
	TankPawn->MoveRight(value);
}

void ATankPlayerController::RotateRight(float value)
{
	TankPawn->RotateRight(value);
}

void ATankPlayerController::Fire()
{
	TankPawn->Fire();
}

void ATankPlayerController::FireSpecial()
{
	TankPawn->FireSpecial();
}

void ATankPlayerController::SwitchFireMode()
{
	TankPawn->SwitchFireMode();
}

void ATankPlayerController::SwitchCannon()
{
	TankPawn->SwitchCannon();
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATankPlayerController::MoveRight);
	InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::RotateRight);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction("FireSpecial", IE_Pressed, this, &ATankPlayerController::FireSpecial);
	InputComponent->BindAction("SwitchFireMode", IE_Pressed, this, &ATankPlayerController::SwitchFireMode);
	InputComponent->BindAction("SwitchCannon", IE_Pressed, this, &ATankPlayerController::SwitchCannon);
}

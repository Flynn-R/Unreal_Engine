// Fill out your copyright notice in the Description page of Project Settings.


#include "AddAmmoBox.h"
#include "TankPawn.h"

void AAddAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* playerPawn =
		Cast<ATankPawn>(OtherActor);
	if (playerPawn)
	{
		playerPawn->GetCannon()->SetAmmo(playerPawn->GetCannon()->GetAmmo() + AmmoCapacity);
		Destroy();
	}
}

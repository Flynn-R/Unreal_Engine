// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoBox.h"
#include "AddAmmoBox.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API AAddAmmoBox : public AAmmoBox
{
	GENERATED_BODY()
protected:
	uint8 AmmoCapacity = 3;

protected:
	virtual void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const
		FHitResult& SweepResult) override;

public:
	AAddAmmoBox() : Super::AAmmoBox() {}
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsComponent.h"
#include "DamageTaker.h"

APhysicsProjectile::APhysicsProjectile()
{
	PhysicsComponent = CreateDefaultSubobject<UPhysicsComponent>(TEXT("Physics component"));
}

void APhysicsProjectile::Start()
{
	MoveVector = GetActorForwardVector() * TrajectorySimulationSpeed;
	CurrentTrajectory = PhysicsComponent->GenerateTrajectory(GetActorLocation(), MoveVector, TrajectorySimulationMaxTime, TrajectorySimulationTimeStep);

	if (bShowTrajectory)
	{
		for (FVector position : CurrentTrajectory)
			DrawDebugSphere(GetWorld(), position, 5.0f, 8, FColor::Purple, true, 1.0f, 0, 2.0f);
	}

	TrajectoryPointIndex = 0;
	Super::Start();
}

void APhysicsProjectile::Move()
{
	FVector currentMoveVector = CurrentTrajectory[TrajectoryPointIndex] - GetActorLocation();
	currentMoveVector.Normalize();
	FVector newLocation = GetActorLocation() + currentMoveVector * MoveSpeed * MoveRate;
	SetActorLocation(newLocation);

	if (FVector::Distance(newLocation, CurrentTrajectory[TrajectoryPointIndex]) <= MoveAccuracy)
	{
		++TrajectoryPointIndex;
		if (TrajectoryPointIndex >= CurrentTrajectory.Num())
		{
			Explode();
			Destroy();
		}
		else
		{
			FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTrajectory[TrajectoryPointIndex]);
			SetActorRotation(newRotation);
		}
	}
}

//void APhysicsProjectile::Explode()
//{
//	FVector startPos = GetActorLocation();
//	FVector endPos = startPos + FVector(0.1f);
//
//	FCollisionShape shape = FCollisionShape::MakeSphere(ExplodeRadius);
//	FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
//	params.AddIgnoredActor(this);
//	params.bTraceComplex = true;
//	params.TraceTag = "Explode trace";
//	TArray<FHitResult> AttackHit;
//
//	FQuat Rotation = FQuat::Identity;
//
//	bool sweepResult = GetWorld()->SweepMultiByChannel(AttackHit, startPos, endPos, Rotation, ECollisionChannel::ECC_Visibility, shape, params);
//	GetWorld()->DebugDrawTraceTag = "Explode trace";
//
//	if (sweepResult)
//	{
//		for (FHitResult hitResult : AttackHit)
//		{
//			AActor* otherActor = hitResult.GetActor();
//			if (!otherActor)
//				continue;
//
//			IDamageTaker* damageTakerActor = Cast<IDamageTaker>(otherActor);
//			if (damageTakerActor)
//			{
//				FDamageData damageData;
//				damageData.DamageValue = Damage;
//				damageData.Instigator = GetOwner();
//				damageData.DamageMaker = this;
//
//				damageTakerActor->TakeDamage(damageData);
//			}
//			else
//			{
//				UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(otherActor->GetRootComponent());
//				if (mesh)
//				{
//					if (mesh->IsSimulatingPhysics())
//					{
//						FVector forceVector = otherActor->GetActorLocation() - GetActorLocation();
//						forceVector.Normalize();
//						/*mesh->AddImpulse(forceVector * PushForce, NAME_None, true);*/
//						mesh->AddForce(forceVector, NAME_None, true);
//					}
//				}
//			}
//		}
//	}
//}

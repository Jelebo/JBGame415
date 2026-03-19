// Copyright Epic Games, Inc. All Rights Reserved.

#include "JBGame415Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"

AJBGame415Projectile::AJBGame415Projectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AJBGame415Projectile::OnHit);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Create a visible mesh for the projectile
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AJBGame415Projectile::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileMesh)
	{
		UMaterialInstanceDynamic* DynMat = ProjectileMesh->CreateAndSetMaterialInstanceDynamic(0);

		if (DynMat)
		{
			RandomColor = FLinearColor(
				FMath::FRand(),
				FMath::FRand(),
				FMath::FRand(),
				1.0f
			);

			DynMat->SetVectorParameterValue(TEXT("Color"), RandomColor);
		}
	}
}

void AJBGame415Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Apply impulse if the hit object simulates physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}

	// Spawn a random decal and tint it to match the projectile color
	if (DecalMaterials.Num() > 0)
	{
		const int32 RandomIndex = FMath::RandRange(0, DecalMaterials.Num() - 1);
		UMaterialInterface* BaseDecalMaterial = DecalMaterials[RandomIndex];

		if (BaseDecalMaterial)
		{
			UMaterialInstanceDynamic* DecalMID = UMaterialInstanceDynamic::Create(BaseDecalMaterial, this);

			if (DecalMID)
			{
				DecalMID->SetVectorParameterValue(TEXT("Color"), RandomColor);
			}

			const FRotator DecalRotation = FRotationMatrix::MakeFromX(Hit.ImpactNormal).Rotator();

			UGameplayStatics::SpawnDecalAtLocation(
				GetWorld(),
				DecalMID ? DecalMID : BaseDecalMaterial,
				DecalSize,
				Hit.ImpactPoint,
				DecalRotation,
				DecalLifeSpan
			);
		}
	}

	Destroy();
}
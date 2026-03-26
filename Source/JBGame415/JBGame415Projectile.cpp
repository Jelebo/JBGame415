// Copyright Epic Games, Inc. All Rights Reserved.

#include "JBGame415Projectile.h"
#include "PerlinProc.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AJBGame415Projectile::AJBGame415Projectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AJBGame415Projectile::OnHit);

	// Players cannot walk on the projectile
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set the collision sphere as the root component
	RootComponent = CollisionComp;

	// Create a visible mesh for the projectile
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create the movement component that controls projectile speed and bounce behavior
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Destroy the projectile automatically after 3 seconds
	InitialLifeSpan = 3.0f;
}

void AJBGame415Projectile::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileMesh)
	{
		// Create a dynamic material instance so the projectile color can be changed at runtime
		UMaterialInstanceDynamic* DynMat = ProjectileMesh->CreateAndSetMaterialInstanceDynamic(0);

		if (DynMat)
		{
			// Generate and store a random color for this projectile
			RandomColor = FLinearColor(
				FMath::FRand(),
				FMath::FRand(),
				FMath::FRand(),
				1.0f
			);

			// Apply the random color to the projectile material
			DynMat->SetVectorParameterValue(TEXT("Color"), RandomColor);
		}
	}
}

void AJBGame415Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// If the projectile hits the procedural terrain, deform it
	APerlinProc* Terrain = Cast<APerlinProc>(OtherActor);
	if (Terrain)
	{
		Terrain->AlterMesh(Hit.ImpactPoint);
	}

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
				// Pass the projectile color into the decal material
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

	// Spawn the Niagara particle system at the hit location
	if (ImpactParticleSystem)
	{
		UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ImpactParticleSystem,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
		);

		if (SpawnedEffect)
		{
			// Pass the projectile color into the particle system so it matches the decal and projectile
			SpawnedEffect->SetVariableLinearColor(TEXT("ParticleColor"), RandomColor);
		}
	}

	Destroy();
}
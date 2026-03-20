// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JBGame415Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UStaticMeshComponent;
class UMaterialInterface;
class UPrimitiveComponent;
class UNiagaraSystem;

UCLASS(config = Game)
class AJBGame415Projectile : public AActor
{
	GENERATED_BODY()

public:
	AJBGame415Projectile();

protected:
	virtual void BeginPlay() override;

	// Collision component used to detect projectile impacts
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	// Static mesh used as the visible projectile
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMesh;

	// Controls projectile movement behavior
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovement;

	// Stores the random color used by the projectile, decal, and particle effect
	UPROPERTY(VisibleAnywhere, Category = Projectile)
	FLinearColor RandomColor;

	// Array of decal materials used to randomize the paint splat decal
	UPROPERTY(EditDefaultsOnly, Category = Decal)
	TArray<UMaterialInterface*> DecalMaterials;

	// Size of the spawned decal on impact
	UPROPERTY(EditDefaultsOnly, Category = Decal)
	FVector DecalSize = FVector(25.0f, 25.0f, 25.0f);

	// How long the decal stays in the world before disappearing
	UPROPERTY(EditDefaultsOnly, Category = Decal)
	float DecalLifeSpan = 10.0f;

	// Niagara particle system spawned when the projectile hits a surface
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UNiagaraSystem* ImpactParticleSystem;

	// Called when the projectile hits another object
	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);
};
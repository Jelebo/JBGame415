// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JBGame415Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UStaticMeshComponent;
class UMaterialInterface;

UCLASS(config = Game)
class AJBGame415Projectile : public AActor
{
	GENERATED_BODY()

public:
	AJBGame415Projectile();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
	FLinearColor RandomColor;

	UPROPERTY(EditDefaultsOnly, Category = Decal)
	TArray<UMaterialInterface*> DecalMaterials;

	UPROPERTY(EditDefaultsOnly, Category = Decal)
	FVector DecalSize = FVector(25.0f, 25.0f, 25.0f);

	UPROPERTY(EditDefaultsOnly, Category = Decal)
	float DecalLifeSpan = 10.0f;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
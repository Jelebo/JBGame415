#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInterface.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ProjectileCPP.generated.h"

UCLASS()
class JBGAME415_API AProjectileCPP : public AActor
{
    GENERATED_BODY()

public:
    AProjectileCPP();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComp;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* ProjectileMesh;

    UPROPERTY(VisibleAnywhere)
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditAnywhere)
    UMaterialInterface* ProjectileMaterial;

    UPROPERTY()
    UMaterialInstanceDynamic* DynamicMaterial;

    UPROPERTY(VisibleAnywhere)
    FLinearColor RandomColor;
};
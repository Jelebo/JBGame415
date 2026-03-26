#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProcPlane.generated.h"

class UProceduralMeshComponent;

UCLASS()
class JBGAME415_API AProcPlane : public AActor
{
	GENERATED_BODY()

public:
	AProcPlane();

protected:
	virtual void BeginPlay() override;
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TArray<FVector> Vertices;

	UPROPERTY(EditAnywhere)
	TArray<int32> Triangles;

	UPROPERTY(EditAnywhere)
	float Size = 100.0f;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* PlaneMat;

	UPROPERTY(EditAnywhere)
	TArray<FVector2D> UV0;

	UFUNCTION()
	void CreateMesh();


private:
	UProceduralMeshComponent* procMesh;
};
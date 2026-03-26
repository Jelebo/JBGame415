#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "PerlinProc.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

UCLASS()
class JBGAME415_API APerlinProc : public AActor
{
	GENERATED_BODY()

public:
	APerlinProc();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void Tick(float DeltaTime) override;

	// === TERRAIN SETTINGS ===
	UPROPERTY(EditAnywhere)
	int XSize;

	UPROPERTY(EditAnywhere)
	int YSize;

	UPROPERTY(EditAnywhere)
	float ZMultiplier;

	UPROPERTY(EditAnywhere)
	float NoiseScale;

	UPROPERTY(EditAnywhere)
	float UVScale;

	UPROPERTY(EditAnywhere)
	float Scale;

	UPROPERTY(EditAnywhere)
	float Radius;

	UPROPERTY(EditAnywhere)
	float Depth;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	// === MESH DATA ===
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	// === COMPONENT ===
	UPROPERTY()
	UProceduralMeshComponent* ProcMesh;

	// === FUNCTIONS ===
	void CreateVertices();
	void CreateTriangles();

	UFUNCTION()
	void AlterMesh(FVector ImpactPoint);
};
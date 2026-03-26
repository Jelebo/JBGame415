// Fill out your copyright notice in the Description page of Project Settings.

#include "ProcPlane.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AProcPlane::AProcPlane()
{
	PrimaryActorTick.bCanEverTick = false;

	procMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("procMesh"));
	RootComponent = procMesh;
}

// Called when the game starts or when spawned
void AProcPlane::BeginPlay()
{
	Super::BeginPlay();
}

// Called after actor is created
void AProcPlane::PostActorCreated()
{
	Super::PostActorCreated();
	CreateMesh();
}

// Called after actor is loaded
void AProcPlane::PostLoad()
{
	Super::PostLoad();
	CreateMesh();
}

void AProcPlane::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	CreateMesh();

	if (PlaneMat)
	{
		procMesh->SetMaterial(0, PlaneMat);
	}
}

// Called every frame
void AProcPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProcPlane::CreateMesh()
{
	Vertices.Empty();
	Triangles.Empty();
	UV0.Empty();

	Vertices.Add(FVector(0.0f, 0.0f, 0.0f));
	Vertices.Add(FVector(0.0f, Size, 0.0f));
	Vertices.Add(FVector(Size, 0.0f, 0.0f));
	Vertices.Add(FVector(Size, Size, 0.0f));

	UV0.Add(FVector2D(0.0f, 0.0f));
	UV0.Add(FVector2D(0.0f, 1.0f));
	UV0.Add(FVector2D(1.0f, 0.0f));
	UV0.Add(FVector2D(1.0f, 1.0f));

	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(2);

	Triangles.Add(2);
	Triangles.Add(1);
	Triangles.Add(3);

	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;

	Normals.Add(FVector(0.0f, 0.0f, 1.0f));
	Normals.Add(FVector(0.0f, 0.0f, 1.0f));
	Normals.Add(FVector(0.0f, 0.0f, 1.0f));
	Normals.Add(FVector(0.0f, 0.0f, 1.0f));

	VertexColors.Add(FLinearColor::White);
	VertexColors.Add(FLinearColor::White);
	VertexColors.Add(FLinearColor::White);
	VertexColors.Add(FLinearColor::White);

	Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
	Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
	Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
	Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));

	procMesh->CreateMeshSection_LinearColor(
		0,
		Vertices,
		Triangles,
		Normals,
		UV0,
		VertexColors,
		Tangents,
		true
	);
}
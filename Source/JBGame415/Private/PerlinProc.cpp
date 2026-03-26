#include "PerlinProc.h"
#include "ProceduralMeshComponent.h"
#include "Materials/MaterialInterface.h"

// Sets default values
APerlinProc::APerlinProc()
{
	PrimaryActorTick.bCanEverTick = true;

	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));
	RootComponent = ProcMesh;

	// Enable collision
	ProcMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProcMesh->SetCollisionObjectType(ECC_WorldStatic);
	ProcMesh->SetCollisionResponseToAllChannels(ECR_Block);

	// Default terrain values
	XSize = 10;
	YSize = 10;
	ZMultiplier = 200.f;
	NoiseScale = 0.5f;
	UVScale = 1.0f;
	Scale = 100.f;
	Radius = 800.f;
	Depth = 400.f;
}

// Called when the game starts
void APerlinProc::BeginPlay()
{
	Super::BeginPlay();

	// Rebuild arrays at runtime so AlterMesh has valid vertex data
	CreateVertices();
	CreateTriangles();

	ProcMesh->CreateMeshSection_LinearColor(
		0,
		Vertices,
		Triangles,
		Normals,
		UVs,
		VertexColors,
		Tangents,
		true
	);

	if (Material)
	{
		ProcMesh->SetMaterial(0, Material);
	}
}

// Builds mesh in editor and runtime
void APerlinProc::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	CreateVertices();
	CreateTriangles();

	ProcMesh->CreateMeshSection_LinearColor(
		0,
		Vertices,
		Triangles,
		Normals,
		UVs,
		VertexColors,
		Tangents,
		true
	);

	if (Material)
	{
		ProcMesh->SetMaterial(0, Material);
	}
}

// Called every frame
void APerlinProc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APerlinProc::CreateVertices()
{
	Vertices.Empty();
	Normals.Empty();
	UVs.Empty();
	VertexColors.Empty();
	Tangents.Empty();

	for (int X = 0; X <= XSize; X++)
	{
		for (int Y = 0; Y <= YSize; Y++)
		{
			float Z = FMath::PerlinNoise2D(FVector2D(X * NoiseScale, Y * NoiseScale)) * ZMultiplier;

			Vertices.Add(FVector(X * Scale, Y * Scale, Z));
			Normals.Add(FVector(0, 0, 1));
			UVs.Add(FVector2D(X * UVScale, Y * UVScale));
			VertexColors.Add(FLinearColor::White);
			Tangents.Add(FProcMeshTangent(1, 0, 0));
		}
	}
}

void APerlinProc::CreateTriangles()
{
	Triangles.Empty();

	for (int X = 0; X < XSize; X++)
	{
		for (int Y = 0; Y < YSize; Y++)
		{
			int Vertex = (X * (YSize + 1)) + Y;

			Triangles.Add(Vertex);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 1);

			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 2);
			Triangles.Add(Vertex + YSize + 1);
		}
	}
}

void APerlinProc::AlterMesh(FVector ImpactPoint)
{
	UE_LOG(LogTemp, Warning, TEXT("AlterMesh called"));
	UE_LOG(LogTemp, Warning, TEXT("Vertices Num: %d"), Vertices.Num());

	// Convert world hit location into the mesh's local space
	FVector LocalHit = ProcMesh->GetComponentTransform().InverseTransformPosition(ImpactPoint);

	int32 ChangedVertices = 0;

	for (int i = 0; i < Vertices.Num(); i++)
	{
		float Distance = FVector::Dist2D(Vertices[i], LocalHit);

		if (Distance < Radius)
		{
			Vertices[i].Z -= Depth;
			ChangedVertices++;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Changed Vertices: %d"), ChangedVertices);

	if (ChangedVertices == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No vertices modified, skipping update"));
		return;
	}

	ProcMesh->UpdateMeshSection_LinearColor(
		0,
		Vertices,
		Normals,
		UVs,
		VertexColors,
		Tangents
	);
}
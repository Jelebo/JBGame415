#include "ProcMeshFromStatic.h"
#include "KismetProceduralMeshLibrary.h"

// Constructor
AProcMeshFromStatic::AProcMeshFromStatic()
{
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));
	ProcMesh->SetupAttachment(BaseMesh);
}

void AProcMeshFromStatic::BeginPlay()
{
	Super::BeginPlay();
}

void AProcMeshFromStatic::PostActorCreated()
{
	Super::PostActorCreated();
	GetMeshData();
}

void AProcMeshFromStatic::PostLoad()
{
	Super::PostLoad();
	GetMeshData();
}

// 🔑 THIS is the key function
void AProcMeshFromStatic::GetMeshData()
{
	if (!BaseMesh || !BaseMesh->GetStaticMesh())
		return;

	UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(
		BaseMesh->GetStaticMesh(),
		0, // LOD
		0, // Section
		Vertices,
		Triangles,
		Normals,
		UV0,
		Tangents
	);

	CreateMesh();
}

void AProcMeshFromStatic::CreateMesh()
{
	if (!BaseMesh) return;

	ProcMesh->CreateMeshSection_LinearColor(
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
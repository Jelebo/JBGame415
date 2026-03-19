#include "ProjectileCPP.h"

// Sets default values
AProjectileCPP::AProjectileCPP()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create collision component
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    RootComponent = CollisionComp;

    // Create mesh component
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(RootComponent);

    // Create movement component
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = CollisionComp;

    ProjectileMaterial = nullptr;
    DynamicMaterial = nullptr;
    RandomColor = FLinearColor::White;
}

// Called when the game starts or when spawned
void AProjectileCPP::BeginPlay()
{
    Super::BeginPlay();

    UStaticMeshComponent* ActualMesh = FindComponentByClass<UStaticMeshComponent>();

    if (ActualMesh)
    {
        // Temporary test color
        RandomColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);

        FVector TestColor(RandomColor.R, RandomColor.G, RandomColor.B);

        // Apply the color parameter directly to all materials on the mesh
        ActualMesh->SetVectorParameterValueOnMaterials(TEXT("Color"), TestColor);
    }
}

// Called every frame
void AProjectileCPP::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
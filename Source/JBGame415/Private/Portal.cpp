#include "Portal.h"
#include "../JBGame415Character.h"
#include "Engine/Engine.h"

// Sets default values
APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Scene Capture
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePortals();
}

void APortal::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Overlap triggered"));
	}

	AJBGame415Character* PlayerCharacter = Cast<AJBGame415Character>(OtherActor);

	if (PlayerCharacter && OtherPortal && !PlayerCharacter->isTeleporting)
	{
		PlayerCharacter->isTeleporting = true;
		PlayerCharacter->SetActorLocation(OtherPortal->GetActorLocation() + FVector(150.f, 0.f, 0.f));

		SetTeleportingFalse(PlayerCharacter);
	}
}

void APortal::UpdatePortals()
{
}

void APortal::SetTeleportingFalse(AJBGame415Character* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		PlayerCharacter->isTeleporting = false;
	}
}
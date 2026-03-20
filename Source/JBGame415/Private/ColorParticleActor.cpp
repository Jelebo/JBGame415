#include "ColorParticleActor.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/PrimitiveComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AColorParticleActor::AColorParticleActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	NiagaraComp->SetupAttachment(RootComponent);

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AColorParticleActor::OnOverlapBegin);
}

void AColorParticleActor::BeginPlay()
{
	Super::BeginPlay();

	if (ParticleSystem)
	{
		NiagaraComp->SetAsset(ParticleSystem);
		NiagaraComp->Activate();
	}
}

void AColorParticleActor::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && ParticleSystem)
	{
		FLinearColor RandomColor = FLinearColor::MakeRandomColor();

		NiagaraComp->SetVariableLinearColor(TEXT("User.ParticleColor"), RandomColor);

		
		NiagaraComp->ReinitializeSystem();
	}
}

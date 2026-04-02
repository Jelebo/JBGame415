#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

class AJBGame415Character;

#include "Portal.generated.h"

UCLASS()
class JBGAME415_API APortal : public AActor
{
	GENERATED_BODY()

public:
	APortal();

protected:
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	USceneCaptureComponent2D* SceneCapture;

	// Portal data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	UTextureRenderTarget2D* RenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	APortal* OtherPortal;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	void UpdatePortals();

	void SetTeleportingFalse(AJBGame415Character* PlayerCharacter);

public:
	virtual void Tick(float DeltaTime) override;
};
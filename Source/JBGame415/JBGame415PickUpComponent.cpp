// Copyright Epic Games, Inc. All Rights Reserved.

#include "JBGame415PickUpComponent.h"

UJBGame415PickUpComponent::UJBGame415PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UJBGame415PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UJBGame415PickUpComponent::OnSphereBeginOverlap);
}

void UJBGame415PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AJBGame415Character* Character = Cast<AJBGame415Character>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}

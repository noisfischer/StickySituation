#include "Components/Grapple/GrappleComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/PlayerCharacterBase.h"
#include "Components/Grapple/GrappleHook.h"
#include "Kismet/GameplayStatics.h"

UGrappleComponent::UGrappleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UGrappleComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UGrappleComponent::ActivateGrapple(FVector StartLocation, FRotator Rotation, FVector EndLocation, float GrappleSpeed)
{
	if(!GrappleHookBlueprint)
	{
		UE_LOG(LogTemp, Warning, TEXT("FAILED TO FIRE GRAPPLE! Must set GrappleHook subclass!"));
	}

	else
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AGrappleHook* GrappleHook = GetWorld()->SpawnActor<AGrappleHook>
		(
			GrappleHookBlueprint,
			StartLocation,
			Rotation,
			SpawnParams
		);

		if(GrappleHook)
			GrappleHook->Initialize(StartLocation, EndLocation, GrappleSpeed);
		else
			UE_LOG(LogTemp, Warning, TEXT("GrappleHook failed to spawn"));
	}
}





/*
void UGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
*/


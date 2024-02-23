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


void UGrappleComponent::ActivateGrapple(FVector StartLocation, FRotator Rotation, FVector Direction)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AGrappleHook* GrappleHook = GetWorld()->SpawnActor<AGrappleHook>
	(
		AGrappleHook::StaticClass(),
		StartLocation,
		Rotation,
		SpawnParams
	);
	
}





/*
void UGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
*/


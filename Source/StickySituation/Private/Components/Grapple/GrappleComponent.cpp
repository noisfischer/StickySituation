#include "Components/Grapple/GrappleComponent.h"
#include "PropertyAccess.h"
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

		ActiveGrappleHook = GetWorld()->SpawnActor<AGrappleHook>
		(
			GrappleHookBlueprint,
			StartLocation,
			Rotation,
			SpawnParams
		);

		if(ActiveGrappleHook)
		{
			ActiveGrappleHook->Initialize(StartLocation, EndLocation, GrappleSpeed);
			ActiveGrappleHook->GrappleMesh->OnComponentHit.AddDynamic(this, &UGrappleComponent::OnGrappleSuccess);
			ActiveGrappleHook->OnDestroyed.AddDynamic(this, &UGrappleComponent::OnGrappleHookDestroyed);
		}

		else
			UE_LOG(LogTemp, Warning, TEXT("GrappleHook failed to spawn"));
	}
}

void UGrappleComponent::OnGrappleSuccess(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FVector GrappleDirection = ActiveGrappleHook->Direction;
	ACharacter* PlayerRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	PlayerRef->LaunchCharacter(FVector (0, 0, 1000), true, true);
	// Launch Character
}

void UGrappleComponent::OnGrappleHookDestroyed(AActor* DestroyedGrappleHook)
{
	if(ActiveGrappleHook && ActiveGrappleHook->bGrappleSuccess)
	{
		ActiveGrappleHook->OnDestroyed.RemoveDynamic(this, &UGrappleComponent::OnGrappleHookDestroyed);
		ActiveGrappleHook->GrappleMesh->OnComponentHit.RemoveDynamic(this, &UGrappleComponent::OnGrappleSuccess);
		ActiveGrappleHook = nullptr;
	}

	else if (ActiveGrappleHook)
	{
		ActiveGrappleHook->GrappleMesh->OnComponentHit.RemoveDynamic(this, &UGrappleComponent::OnGrappleSuccess);
		ActiveGrappleHook = nullptr;
	}
}


/*
void UGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
*/


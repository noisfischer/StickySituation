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


void UGrappleComponent::ActivateGrapple(FVector StartLocation, FRotator Rotation, FVector EndLocation)
{
	if(!GrappleHookBlueprint)
	{
		UE_LOG(LogTemp, Warning, TEXT("FAILED TO FIRE GRAPPLE! Must set GrappleHook subclass!"));
	}

	else if (ActiveGrappleHook == nullptr)
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
			bool const bInitSuccess = ActiveGrappleHook->Initialize(StartLocation, EndLocation, GrappleSpeed);
			ensure(bInitSuccess);
			
			ActiveGrappleHook->GrappleMesh->OnComponentHit.AddDynamic(this, &UGrappleComponent::OnGrappleSuccess);
			ActiveGrappleHook->OnDestroyed.AddDynamic(this, &UGrappleComponent::OnGrappleHookDestroyed);
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GrappleHook failed to spawn"));
		}
			
	}
}

void UGrappleComponent::OnGrappleSuccess(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(ActiveGrappleHook)
	{
		ACharacter* PlayerRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		FVector Direction = (ActiveGrappleHook->HitLocation - PlayerRef->GetActorLocation()).GetSafeNormal();
		FVector Velocity = Direction * (GrappleLaunchPower * ActiveGrappleHook->LaunchMultiplier);
		PlayerRef->LaunchCharacter(Velocity, true, true);
		ActiveGrappleHook->Destroy();
	}
}

void UGrappleComponent::OnGrappleHookDestroyed(AActor* DestroyedGrappleHook)
{
	if(ActiveGrappleHook)
	{
		ActiveGrappleHook->OnDestroyed.RemoveDynamic(this, &UGrappleComponent::OnGrappleHookDestroyed);
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


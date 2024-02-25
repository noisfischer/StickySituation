#include "Components/Grapple/GrappleHook.h"

AGrappleHook::AGrappleHook()
{
	PrimaryActorTick.bCanEverTick = true;

	GrappleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrappleMesh"));
	RootComponent = GrappleMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	ProjectileMovementComponent->ProjectileGravityScale = 0;
}


bool AGrappleHook::Initialize(const FVector& NewStartLocation, const FVector& NewEndLocation, const float NewSpeed)
{
	StartLocation = NewStartLocation;
	EndLocation = NewEndLocation;
    Direction = (EndLocation - StartLocation).GetSafeNormal();
	ProjectileMovementComponent->Velocity = Direction * NewSpeed;

	PerformLineTrace();

	return true;
}

void AGrappleHook::PerformLineTrace()
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		CollisionParams
		);

	if(bHit)
	{
		HitLocation = HitResult.Location;
		float DistanceToHit = FVector::Dist(StartLocation, HitLocation);
		float TotalDistance = FVector::Dist(StartLocation, EndLocation);
		if(TotalDistance > 0)
			LaunchMultiplier = FMath::Clamp(DistanceToHit/TotalDistance, .35, 1);
	}
		
	
	DrawDebugLine(
		GetWorld(),
		StartLocation,
		EndLocation,
		FColor::Red,
		false,
		1,
		0,
		1
		);
}

/*
void AGrappleHook::BeginPlay()
{
	Super::BeginPlay();
}
*/


void AGrappleHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(FVector::DistSquared(GetActorLocation(), EndLocation) <= TargetRadiusSquared)
		Destroy();
}
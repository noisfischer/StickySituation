#include "Components/Grapple/GrappleHook.h"

AGrappleHook::AGrappleHook()
{
	PrimaryActorTick.bCanEverTick = true;

	GrappleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrappleMesh"));
	RootComponent = GrappleMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	ProjectileMovementComponent->ProjectileGravityScale = 0;
}


void AGrappleHook::Initialize(const FVector& NewStartLocation, const FVector& NewEndLocation, const float NewSpeed)
{
	StartLocation = NewStartLocation;
	EndLocation = NewEndLocation;
    Direction = (EndLocation - StartLocation).GetSafeNormal();
	ProjectileMovementComponent->Velocity = Direction * NewSpeed;

	PerformLineTrace();
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
		HitLocation = HitResult.Location;
	
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
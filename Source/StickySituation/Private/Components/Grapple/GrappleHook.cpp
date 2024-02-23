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
    FVector Direction = (EndLocation - StartLocation).GetSafeNormal(); // Correctly obtain normalized direction
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

void AGrappleHook::BeginPlay()
{
	Super::BeginPlay();

	GrappleMesh->OnComponentHit.AddDynamic(this, &AGrappleHook::OnHit);
}


void AGrappleHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(FVector::DistSquared(GetActorLocation(), EndLocation) <= TargetRadiusSquared)
		Destroy();
}


void AGrappleHook::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	bGrappleSuccess = true;
	this->Destroy();
}

#include "Components/Grapple/GrappleHook.h"

AGrappleHook::AGrappleHook()
{
	PrimaryActorTick.bCanEverTick = true;

	GrappleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrappleMesh"));
	RootComponent = GrappleMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	ProjectileMovementComponent->ProjectileGravityScale = 0;
}

void AGrappleHook::BeginPlay()
{
	Super::BeginPlay();

	GrappleMesh->OnComponentHit.AddDynamic(this, &AGrappleHook::OnHit);
}


void AGrappleHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrappleHook::Initialize(const FVector& NewStartLocation, const FVector& NewEndLocation, const float NewSpeed)
{
	StartLocation = NewStartLocation;
	EndLocation = NewEndLocation;
	ProjectileMovementComponent->MaxSpeed = NewSpeed;
	ProjectileMovementComponent->InitialSpeed = NewSpeed;
}

void AGrappleHook::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	this->Destroy();
}

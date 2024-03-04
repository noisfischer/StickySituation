// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/GreenProjectile.h"

AGreenProjectile::AGreenProjectile()
{
	HomingDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HomingDetection"));
	HomingDetectionSphere->SetupAttachment(RootComponent);
	HomingDetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->bIsHomingProjectile = true;
	ProjectileMovementComponent->HomingAccelerationMagnitude = 10000.f;
}


void AGreenProjectile::BeginPlay()
{
	Super::BeginPlay();

	HomingDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AGreenProjectile::HomingDetectionBeginOverlap);
}

void AGreenProjectile::HomingDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("enemy"))
	{
		ProjectileMovementComponent->HomingTargetComponent = OtherActor->GetRootComponent();
		HomingDetectionSphere->OnComponentBeginOverlap.RemoveDynamic(this, &AGreenProjectile::HomingDetectionBeginOverlap);
		UE_LOG(LogTemp, Warning, TEXT("Target Locked"));
		HomingDetectionSphere->DestroyComponent();
		ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileMovementComponent->InitialSpeed;
	}
}

void AGreenProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* HitActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComponent, HitActor, OtherComp, NormalImpulse, Hit);
	PerformRadialBlast(BlastRadius); // iterates through hit enemies and performs RadialBlastAction()
	this->Destroy();
}

void AGreenProjectile::RadialBlastAction(AActor* Enemy)
{
	Super::RadialBlastAction(Enemy);
	Execute_DamageEnemy(Enemy, Damage);
}


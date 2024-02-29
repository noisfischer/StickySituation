// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/BlueProjectile.h"

ABlueProjectile::ABlueProjectile()
{
	//
}

void ABlueProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ABlueProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* HitActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComponent, HitActor, OtherComp, NormalImpulse, Hit);

	PerformRadialBlast(BlastRadius);	// calls RadialBlastAction
}

void ABlueProjectile::RadialBlastAction(AActor* Enemy)
{
	Super::RadialBlastAction(Enemy);

	Execute_StunEnemy(Enemy);
}

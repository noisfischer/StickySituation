// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/BlueProjectile.h"

ABlueProjectile::ABlueProjectile()
{
}

void ABlueProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ABlueProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* HitActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComponent, HitActor, OtherComp, NormalImpulse, Hit);
	PerformRadialBlast(BlastRadius); // iterates through hit enemies and performs RadialBlastAction()
	this->Destroy();
}

void ABlueProjectile::RadialBlastAction(AActor* Enemy)
{
	Super::RadialBlastAction(Enemy);

	float RandomNum = FMath::RandRange(0, 1);
	if(RandomNum <= StunChance)		// Default 50% chance (no skill active)
		Execute_StunEnemy(Enemy, StunTime);

	UE_LOG(LogTemp, Warning, TEXT("Stun Chance: %f"), StunChance);
	UE_LOG(LogTemp, Warning, TEXT("Stun Time: %f"), StunTime);
}

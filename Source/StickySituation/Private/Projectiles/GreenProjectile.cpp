// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/GreenProjectile.h"

AGreenProjectile::AGreenProjectile()
{
	//
}

void AGreenProjectile::BeginPlay()
{
	Super::BeginPlay();
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


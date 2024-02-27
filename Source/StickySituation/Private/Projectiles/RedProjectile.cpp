// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/RedProjectile.h"

ARedProjectile::ARedProjectile()
{
	//
}

void ARedProjectile::BeginPlay()
{
	Super::BeginPlay();
}


void ARedProjectile::Explosion()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	
	GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(BlastRadius),
		QueryParams
		);

	DrawDebugSphere(
		GetWorld(),
		GetActorLocation(),
		BlastRadius,
		50,
		FColor::Red,
		false,
		2.f,
		0
		);

	if(HitResults.Num() > 0)
	{
		for(auto Hit : HitResults)
		{
			if(Hit.GetActor()->ActorHasTag("enemy"))
			{
				UE_LOG(LogTemp, Warning, TEXT("Enemy Hit!"));
			}
		}
	}
		
}

void ARedProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* HitActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComponent, HitActor, OtherComp, NormalImpulse, Hit);

	Explosion();
}


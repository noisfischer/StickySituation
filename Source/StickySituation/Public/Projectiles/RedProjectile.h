// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/ProjectileBase.h"
#include "RedProjectile.generated.h"

UCLASS()
class STICKYSITUATION_API ARedProjectile : public AProjectileBase
{
	GENERATED_BODY()
	
public:
	ARedProjectile();

private:
	virtual void BeginPlay() override;
	
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* HitActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};

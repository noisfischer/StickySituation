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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1 - DAMAGE")
	float BlastRadius = 250.f;

private:
	virtual void BeginPlay() override;
	
	void Explosion();
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* HitActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};

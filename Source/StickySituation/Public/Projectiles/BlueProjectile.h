// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/ProjectileBase.h"
#include "BlueProjectile.generated.h"

/**
 * 
 */
UCLASS()
class STICKYSITUATION_API ABlueProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	ABlueProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StunProperties")
	float StunTime = 5.0f;

private:
	virtual void BeginPlay() override;
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* HitActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void RadialBlastAction(AActor* Enemy) override;
};

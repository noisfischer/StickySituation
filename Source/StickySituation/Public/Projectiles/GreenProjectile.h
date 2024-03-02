#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Projectiles/ProjectileBase.h"
#include "GreenProjectile.generated.h"


UCLASS()
class STICKYSITUATION_API AGreenProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	AGreenProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	USphereComponent* HomingDetectionSphere;

	UFUNCTION()
	void HomingDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

private:
	virtual void BeginPlay() override;
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* HitActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void RadialBlastAction(AActor* Enemy) override;
};

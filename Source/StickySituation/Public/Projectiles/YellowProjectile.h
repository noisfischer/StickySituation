#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Projectiles/ProjectileBase.h"
#include "YellowProjectile.generated.h"

UCLASS()
class STICKYSITUATION_API AYellowProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	AYellowProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	USphereComponent* SphereCollision;

	TArray<AActor*> AffectedEnemies;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1 - DAMAGE")
	FTimerHandle AcidDurationTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1 - DAMAGE")
	FTimerHandle AcidDamageTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1 - DAMAGE")
	float AcidDuration = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1 - DAMAGE")
	float AcidDamageRate = 25.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1 - DAMAGE")
	float AcidDamageAmount = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1 - DAMAGE")
	float AcidScale = 1.f;
	UFUNCTION()
	void EndAcidTimer();
	
	UFUNCTION()
	void ApplyAcidDamage();
	
	UFUNCTION()
	void AcidBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AcidEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	
private:
	virtual void BeginPlay() override;
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* HitActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};

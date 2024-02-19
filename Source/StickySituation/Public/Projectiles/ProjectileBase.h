#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interfaces/CharacterInterface.h"
#include "ProjectileBase.generated.h"

class UNiagaraSystem;

UCLASS()
class STICKYSITUATION_API AProjectileBase : public AActor, public ICharacterInterface
{
	GENERATED_BODY()

	
public:	
	// CONSTRUCTOR //
	AProjectileBase();

	// may need to be changed to skeletal mesh comp for Gummy Bear ammo //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1 - MESH")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1 - MESH")
	UArrowComponent* ForwardArrow;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2 - MOVEMENT")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3 - VFX")
	UNiagaraSystem* ProjectileImpactSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4 - SFX")
	USoundBase* ProjectileImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="5 - DAMAGE")
	float BaseDamage = 0;

	// BOUND TO "OnComponentHit" OF "ProjectileMesh" //
	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* HitActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
		);

	UFUNCTION()
	float GetBaseDamage();

	UFUNCTION()
	virtual void SetBaseDamage(float ChargeValue);

	UFUNCTION()
	float GetCurrentSpeed();

	UFUNCTION()
	virtual void SetCurrentSpeed(float ChargeValue);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};

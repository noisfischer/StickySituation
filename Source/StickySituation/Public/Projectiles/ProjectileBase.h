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
	USoundBase* DirectImpactSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4 - SFX")
	USoundBase* DestroyedSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1 - DAMAGE")
	float Damage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1 - DAMAGE")
	float BlastRadius = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StunProperties")
	float StunTime = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="STUN")
	float StunChance = 0.5f;

	UFUNCTION()
	virtual void PerformRadialBlast(float Radius);
	UFUNCTION()
	virtual void RadialBlastAction(AActor* Enemy);
	
	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* HitActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
		);

	UFUNCTION()
	float GetDamage();
	UFUNCTION()
	virtual void SetDamage(float NewDamage);
	UFUNCTION()
	float GetCurrentSpeed();
	UFUNCTION()
	virtual void SetCurrentSpeed(float NewSpeed);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};

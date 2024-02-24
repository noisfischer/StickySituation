#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GrappleHook.generated.h"

UCLASS()
class STICKYSITUATION_API AGrappleHook : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrappleHook();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* GrappleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grapple")
	FVector StartLocation {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grapple")
	FVector EndLocation {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grapple")
	FVector HitLocation {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grapple")
	FVector Direction {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grapple")
	float LaunchMultiplier {};

	bool bGrappleSuccess = false;
	
	UFUNCTION()
	void Initialize(const FVector& NewStartLocation,
		const FVector& NewEndLocation,
		const float NewSpeed
		);

	UFUNCTION()
	void PerformLineTrace();

protected:
	//virtual void BeginPlay() override;

	const float TargetRadiusSquared = FMath::Square(100.f);

public:	
	virtual void Tick(float DeltaTime) override;

};

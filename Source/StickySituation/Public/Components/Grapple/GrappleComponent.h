#pragma once

#include "CoreMinimal.h"
#include "GrappleHook.h"
#include "Components/ActorComponent.h"
#include "GrappleComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STICKYSITUATION_API UGrappleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGrappleComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grapple")
	TSubclassOf<AGrappleHook> GrappleHookBlueprint;

	UFUNCTION()
	virtual void ActivateGrapple(
		FVector StartLocation,
		FRotator Rotation,
		FVector EndLocation,
		float GrappleSpeed
		);

	UFUNCTION()
	void OnGrappleHookDestroyed(AActor* DestroyedGrappleHook);

protected:
	virtual void BeginPlay() override;

public:
	/*
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	*/
};

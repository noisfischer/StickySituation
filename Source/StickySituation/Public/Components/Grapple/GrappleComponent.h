#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrappleComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STICKYSITUATION_API UGrappleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGrappleComponent();

	UFUNCTION()
	virtual void ActivateGrapple(
		FVector StartLocation,
		FRotator Rotation,
		FVector Direction
		);

protected:
	virtual void BeginPlay() override;

public:
	/*
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	*/
};

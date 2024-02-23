#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrappleComponent.generated.h"


class APlayerCharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STICKYSITUATION_API UGrappleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGrappleComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	APlayerCharacterBase* PlayerRef;

public:
	/*
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	*/
};

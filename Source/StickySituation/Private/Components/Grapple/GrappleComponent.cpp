#include "Components/Grapple/GrappleComponent.h"
#include "Characters/PlayerCharacterBase.h"

UGrappleComponent::UGrappleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	PlayerRef = Cast<APlayerCharacterBase>(GetOwner());
}


void UGrappleComponent::BeginPlay()
{
	Super::BeginPlay();

	check(PlayerRef);
}


/*
void UGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
*/


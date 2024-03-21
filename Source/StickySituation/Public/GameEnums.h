#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Neutral UMETA(DisplayName = "Neutral"),
	CanInteract UMETA(DisplayName = "CanInteract"),
	Busy UMETA(DisplayName = "Busy"),
	Dead UMETA(DisplayName = "Dead")
};
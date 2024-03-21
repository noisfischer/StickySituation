#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Neutral UMETA(DisplayName = "Neutral"),
	CanInteract UMETA(DisplayName = "CanInteract"),
	Hub UMETA(DisplayName = "Hub"),
	Busy UMETA(DisplayName = "Busy"),
	Dead UMETA(DisplayName = "Dead")
};
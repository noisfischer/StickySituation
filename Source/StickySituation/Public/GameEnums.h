#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Neutral UMETA(DisplayName = "Neutral"),
	Busy UMETA(DisplayName = "Busy"),
	Dead UMETA(DisplayName = "Dead")
};
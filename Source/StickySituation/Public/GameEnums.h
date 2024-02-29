#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	Red  UMETA(DisplayName = "Red"),
	Green  UMETA(DisplayName = "Green"),
	Blue  UMETA(DisplayName = "Blue"),
	Yellow  UMETA(DisplayName = "Yellow"),
};
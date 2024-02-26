#pragma once

#include "CoreMinimal.h"
#include "GameStructs.generated.h"

USTRUCT(Blueprintable)
struct FProjectileAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileDamage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileSpeed = 0;
};
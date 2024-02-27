#pragma once

#include "CoreMinimal.h"
#include "Projectiles/ProjectileBase.h"
#include "GameStructs.generated.h"

USTRUCT(BlueprintType)
struct FProjectileData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AProjectileBase> ProjectileClass = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmmoCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 0;
};
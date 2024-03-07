#pragma once

#include "CoreMinimal.h"
#include "Projectiles/ProjectileBase.h"
#include "GameStructs.generated.h"

USTRUCT(BlueprintType)
struct FProjectileData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AProjectileBase> ProjectileClass = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmmoCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 0;
};

USTRUCT(BlueprintType)
struct FCharacterSkills : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUnlocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActive = false;
};

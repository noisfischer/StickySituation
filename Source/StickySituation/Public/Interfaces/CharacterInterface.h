// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class STICKYSITUATION_API ICharacterInterface
{
	GENERATED_BODY()

public:

	// DAMAGE AND HEALTH EVENTS //
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DamagePlayer(const float Damage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HealPlayer(const float Heal);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DamageEnemy(const float Damage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StunEnemy(const float StunTime);
};

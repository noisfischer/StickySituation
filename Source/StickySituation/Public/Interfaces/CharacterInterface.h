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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DamagePlayer(float Damage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HealPlayer(float Heal);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DamageEnemy(float Damage);
	
};

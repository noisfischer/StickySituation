#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveCharacterData.generated.h"


UCLASS()
class STICKYSITUATION_API USaveCharacterData : public USaveGame
{
	GENERATED_BODY()

public:
	USaveCharacterData();
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<FString> CharacterActiveSkills {};

	UFUNCTION(BlueprintCallable)
	void SaveData(TArray<FString> SkillsList);
};

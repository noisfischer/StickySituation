#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "GameFramework/SaveGame.h"
#include "SaveCharacterData.generated.h"


UCLASS()
class STICKYSITUATION_API USaveCharacterData : public USaveGame
{
	GENERATED_BODY()

public:
	USaveCharacterData();
	
	UPROPERTY(VisibleAnywhere, Category = "Character")
	TArray<FString> SavedSkills;

	UPROPERTY(VisibleAnywhere, Category="Character")
	TMap<FString, FCharacterSkills> SavedSkillTreeState;

	UPROPERTY(VisibleAnywhere, Category = "Character")
	float CharacterXP;

	UPROPERTY(VisibleAnywhere, Category = "Character")
	float CharacterLevel;

	UPROPERTY(VisibleAnywhere, Category = "Character")
	float Currency;
};

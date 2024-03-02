#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CurrentGameInstance.generated.h"

UCLASS()
class STICKYSITUATION_API UCurrentGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCurrentGameInstance();

	virtual void Init() override;

	UPROPERTY()
	TArray<FString> CharacterSkillsList;

	void CreateSaveFile();
	void SaveGame();
	void LoadGame();
};

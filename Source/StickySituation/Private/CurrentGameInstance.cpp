#include "CurrentGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveCharacterData.h"

UCurrentGameInstance::UCurrentGameInstance()
{
}

void UCurrentGameInstance::Init()
{
	Super::Init();

	LoadGame();
}

void UCurrentGameInstance::CreateSaveFile()
{
	USaveCharacterData* dataToSave = Cast<USaveCharacterData>(UGameplayStatics::CreateSaveGameObject(USaveCharacterData::StaticClass()));
	UGameplayStatics::SaveGameToSlot(dataToSave, "Slot1", 0);
}

void UCurrentGameInstance::SaveGame()
{
	USaveCharacterData* dataToSave = Cast<USaveCharacterData>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

	if(dataToSave != nullptr)
	{
		dataToSave->CharacterActiveSkills;
		UGameplayStatics::SaveGameToSlot(dataToSave, "Slot1", 0);
	}
	else if(!UGameplayStatics::DoesSaveGameExist("Slot1", 0))
	{
		CreateSaveFile();
	}
}

void UCurrentGameInstance::LoadGame()
{
	USaveCharacterData* dataToLoad = Cast<USaveCharacterData>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

	if(dataToLoad != nullptr)
	{
		CharacterSkillsList = dataToLoad->CharacterActiveSkills;
	}
	else if(!UGameplayStatics::DoesSaveGameExist("Slot1", 0))
	{
		CreateSaveFile();
	}
}

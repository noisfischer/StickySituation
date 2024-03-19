#include "HubInteractables/ShopHub.h"

#include "Kismet/GameplayStatics.h"

AShopHub::AShopHub()
{
}

void AShopHub::BeginPlay()
{
	Super::BeginPlay();

	LoadShopData();
}

void AShopHub::SaveShopData()
{
	USaveCharacterData* SaveGameInstance = Cast<USaveCharacterData>(UGameplayStatics::CreateSaveGameObject(USaveCharacterData::StaticClass()));
	
	SaveGameInstance->SavedShopItemStates = ShopDataMap;
	
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("ShopData"), 0);
	UE_LOG(LogTemp, Warning, TEXT("Saved Shop Data"));
}

void AShopHub::LoadShopData()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("ShopData"), 0))
	{
		USaveCharacterData* LoadGameInstance = Cast<USaveCharacterData>(UGameplayStatics::LoadGameFromSlot(TEXT("ShopData"), 0));
		if (LoadGameInstance != nullptr)
			ShopDataMap = LoadGameInstance->SavedShopItemStates;
	}
	else
	{
		UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTables/ShopData"));
		if (!DataTable)
		{
			UE_LOG(LogTemp, Error, TEXT("ShopData DataTable not found."));
			return;
		}

		static const FString ContextString(TEXT("Projectile Data Loading"));
		ShopDataMap.Empty();
		for (const auto& Row : DataTable->GetRowMap())
		{
			FName RowName = Row.Key;
			FShopItems* RowData = (FShopItems*)Row.Value;

			if (RowData)
			{
				FString RowNameStr = RowName.ToString();
				ShopDataMap.Add(RowNameStr, *RowData);
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("ShopData loaded successfully."));
	}
}

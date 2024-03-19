#pragma once

#include "CoreMinimal.h"
#include "HubInteractables/InteractableHubBase.h"
#include "ShopHub.generated.h"

UCLASS()
class STICKYSITUATION_API AShopHub : public AInteractableHubBase
{
	GENERATED_BODY()
	
public:
	AShopHub();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShopData")
	TMap<FString, FShopItems> ShopDataMap;
	
	UFUNCTION(BlueprintCallable)
	virtual void SaveShopData();
	UFUNCTION(BlueprintCallable)
	virtual void LoadShopData();
};

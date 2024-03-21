#pragma once

#include "CoreMinimal.h"
#include "GameModes/StickySituationGameMode.h"
#include "Characters/PlayerCharacterBase.h"
#include "HubGameMode.generated.h"

UCLASS()
class STICKYSITUATION_API AHubGameMode : public AStickySituationGameMode
{
	GENERATED_BODY()

public:
	AHubGameMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	APlayerCharacterBase* PlayerRef = nullptr;

	virtual void BeginPlay() override;
};

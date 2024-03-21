#include "GameModes/HubGameMode.h"
#include "Kismet/GameplayStatics.h"

AHubGameMode::AHubGameMode()
{
}

void AHubGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<APlayerCharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0));
	ensure(PlayerRef);

	PlayerRef->CurrentState = EPlayerState::Hub;

	UE_LOG(LogTemp, Warning, TEXT("HubGameMode Active"));
}
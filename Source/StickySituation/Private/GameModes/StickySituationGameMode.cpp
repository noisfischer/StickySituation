// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/StickySituationGameMode.h"
#include "Characters/PlayerCharacterBase.h"
#include "UObject/ConstructorHelpers.h"

AStickySituationGameMode::AStickySituationGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Characters/BP_AmyCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

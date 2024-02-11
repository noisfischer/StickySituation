// Copyright Epic Games, Inc. All Rights Reserved.

#include "StickySituationGameMode.h"
#include "StickySituationCharacter.h"
#include "UObject/ConstructorHelpers.h"

AStickySituationGameMode::AStickySituationGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

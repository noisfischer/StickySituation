#pragma once

class GameMacros
{
public:
	
};


#define PRACTICE_MACRO(INT) \
do { \
	if (INT < 0)	\
		UE_LOG(LogTemp, Warning, TEXT("Less than 0"));	\
} while (0)


// Should only be used in classes derived from PlayerCharacterBase
#define DECREMENT_AMMO(E_AMMO_NAME) \
do { \
	auto* FoundData = AmmoMap.Find(E_AMMO_NAME); \
	if (FoundData != nullptr) { \
		if (FoundData->AmmoCount > 0) { \
			FoundData->AmmoCount--; \
		}	\
		else { \
			UE_LOG(LogTemp, Warning, TEXT("Ammo type '%s' has no ammo left to decrement."), TEXT(#E_AMMO_NAME)); \
		} \
	}	\
	else { \
	UE_LOG(LogTemp, Warning, TEXT("Ammo type '%s' not found."), TEXT(#E_AMMO_NAME)); \
	} \
} while (0)
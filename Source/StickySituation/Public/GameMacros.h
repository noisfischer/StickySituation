#pragma once

class GameMacros
{
public:
	
};

// Should only be used in classes derived from PlayerCharacterBase
#define DECREMENT_AMMO(E_AMMO_NAME) \
do { \
	auto* FoundData = ProjectileMap.Find(E_AMMO_NAME); \
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


#define GET_SKILL_VALUE(SkillName) \
	(CharacterSkillTreeComponent->CharacterSkillMap.Find(SkillName) ? CharacterSkillTreeComponent->CharacterSkillMap.Find(SkillName)->Value : 0)

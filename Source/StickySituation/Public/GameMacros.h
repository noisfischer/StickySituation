#pragma once

class GameMacros
{
public:
	
};

// Should only be used in classes derived from PlayerCharacterBase
	// PlayerCharacterBase has CurrentProjectile and ProjectileMap variables!
#define EQUIP_AMMO_TYPE(E_AMMO_TYPE) \
do { \
	TSubclassOf<AProjectileBase>* TemporaryProjectile = ProjectileMap.Find(E_AMMO_TYPE); \
	if(TemporaryProjectile) \
	CurrentProjectile = *TemporaryProjectile; \
} while (0)
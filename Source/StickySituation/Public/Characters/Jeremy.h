// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PlayerCharacterBase.h"
#include "Jeremy.generated.h"

/**
 * 
 */
UCLASS()
class STICKYSITUATION_API AJeremy : public APlayerCharacterBase
{
	GENERATED_BODY()

public:
	// CONSTRUCTOR //
	AJeremy();

	// JEREMY'S UNIQUE INPUT ACTIONS //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* JeremyInputMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SlingshotAction = nullptr;	// MUST BE ASSIGNED IN CHILD BP //

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AmmoSlotAction_1 = nullptr;	// MUST BE ASSIGNED IN CHILD BP //

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AmmoSlotAction_2 = nullptr;	// MUST BE ASSIGNED IN CHILD BP //

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AmmoSlotAction_3 = nullptr;	// MUST BE ASSIGNED IN CHILD BP //

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AmmoSlotAction_4 = nullptr;	// MUST BE ASSIGNED IN CHILD BP //


	// SLINGSHOT ATTRIBUTES //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slingshot")
	float ChargeValue = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slingshot")
	bool bChargeFull = false;

	
	// USED TO SET THE CURRENT AMMO TYPE //
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AProjectileBase> ProjectileClass;

	
	// SFX - assign in child BP //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* SlingshotPull = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* SlingshotFullCharge = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* SlingshotFired = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* SwitchAmmo = nullptr;

	

protected:

	// OVERRIDE FOR MOVE, LOOK, AND JUMP FUNCTIONS FROM BASE CLASS //
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;
	
	
	void StartSlingshotPull();
	void ChargeSlingshot();
	void FireSlingshot();
	void SpawnProjectile(float Multiplier);

	void EquipAmmoSlot1();
	void EquipAmmoSlot2();
	void EquipAmmoSlot3();
	void EquipAmmoSlot4();

	void PlaySound(USoundBase* Sound);
	
};

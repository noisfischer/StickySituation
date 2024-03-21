// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PlayerCharacterBase.h"
#include "Components/Grapple/GrappleComponent.h"
#include "Jeremy.generated.h"

UCLASS()
class STICKYSITUATION_API AJeremy : public APlayerCharacterBase
{
	GENERATED_BODY()

public:
	AJeremy();

	// INPUT ACTIONS - assign all in child BP //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* JeremyInputMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SlingshotAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AmmoSlotAction_1 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AmmoSlotAction_2 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AmmoSlotAction_3 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AmmoSlotAction_4 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* GrappleAction = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grapple")
	UGrappleComponent* GrappleComponent;
	
	// SFX - assign in child BP //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* SlingshotPull = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* SlingshotFullCharge = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* SlingshotFired = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* SwitchAmmo = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* GrappleUnavailable = nullptr;

protected:

	// OVERRIDE FOR MOVE, LOOK, AND JUMP FUNCTIONS FROM BASE CLASS //
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;
	
	// SLINGSHOT ATTRIBUTES //
	float ChargeValue = 0;
	bool bChargeFull = false;
	bool bCharging = false;
	
	void StartSlingshotPull();
	void ChargeSlingshot();
	void FireSlingshot();

	void InitializeAmmo();
	
	void EquipAmmoSlot1();
	void EquipAmmoSlot2();
	void EquipAmmoSlot3();
	void EquipAmmoSlot4();

	float RefreshGrappleTime = 1.f;
	bool bGrappleAvailable = true;
	FTimerHandle GrappleRefreshTimer;
	void Grapple();
	void GrappleRefresh();
	
	virtual void InitializeUnlockedSkills() override;
};

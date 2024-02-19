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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slingshot")
	float FOVValue = 90;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AProjectileBase> ProjectileClass;


protected:

	// OVERRIDE FOR MOVE, LOOK, AND JUMP FUNCTIONS FROM BASE CLASS //
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;
	
	
	void StartSlingshotPull();
	void ChargeSlingshot();
	void FireSlingshot();
	void SpawnProjectile();
	
};

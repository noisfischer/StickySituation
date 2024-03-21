// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CharacterInterface.h"
#include "EnemyBase.generated.h"

UCLASS()
class STICKYSITUATION_API AEnemyBase : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	// CONSTRUCTOR //
	AEnemyBase();


	// COMMON ATTRIBUTES //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1 - HEALTH")
	float MaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1 - HEALTH")
	float CurrentHealth = MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1 - HEALTH")
	bool Dead = false;
	
	
	
	// INTERFACE EVENTS //
	UFUNCTION()
	virtual void DamageEnemy_Implementation(const float Damage) override;
	
	UFUNCTION()
	virtual void StunEnemy_Implementation(const float StunTime) override;
	
	UFUNCTION()
	virtual void StunFinished();
	
	FTimerHandle StunTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/*virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;*/

};

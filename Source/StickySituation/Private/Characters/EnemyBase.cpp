// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyBase.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("enemy");
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// INTERFACE EVENTS //
void AEnemyBase::DamageEnemy_Implementation(float Damage)
{
	CurrentHealth -= Damage;
	UE_LOG(LogTemp, Warning, TEXT("Enemy Current Health: %f"), CurrentHealth);
}

void AEnemyBase::AcidDamage_Implementation(float Damage)
{
	CurrentHealth -= Damage;
	UE_LOG(LogTemp, Warning, TEXT("Enemy Current Health: %f"), CurrentHealth);
}

void AEnemyBase::StunEnemy_Implementation(float StunTime)
{
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetWorld()->GetTimerManager().SetTimer(StunTimer, this, &AEnemyBase::StunFinished, StunTime, false);
	UE_LOG(LogTemp, Warning, TEXT("Enemy Stunned"));
}

void AEnemyBase::StunFinished()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	UE_LOG(LogTemp, Warning, TEXT("Enemy Un-Stunned"));
}


/*void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}*/


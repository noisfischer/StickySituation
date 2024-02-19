// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
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



/*void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}*/


// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Jeremy.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Projectiles/ProjectileBase.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/ProjectileBase.h"


AJeremy::AJeremy()
{
	
}

void AJeremy::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(GetController());
	if(PC && JeremyInputMappingContext)
	{
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if(InputSubsystem)
		{
			InputSubsystem->AddMappingContext(JeremyInputMappingContext, 1); // Priority can be adjusted as needed
		}
	}
}


void AJeremy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// ENABLES MOVE, LOOK, JUMP FROM PARENT CLASS //
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(SlingshotAction, ETriggerEvent::Started, this, &AJeremy::StartSlingshotPull);
		EnhancedInputComponent->BindAction(SlingshotAction, ETriggerEvent::Ongoing, this, &AJeremy::ChargeSlingshot);
		EnhancedInputComponent->BindAction(SlingshotAction, ETriggerEvent::Completed, this, &AJeremy::FireSlingshot);
	}
}

void AJeremy::StartSlingshotPull()
{
	UE_LOG(LogTemp, Warning, TEXT("Slingshot Charge Begins"));
}

void AJeremy::ChargeSlingshot()
{
	UE_LOG(LogTemp, Warning, TEXT("Slingshot Charging"));
	/*FOVValue -= 1;
	FOVValue = FMath::Clamp(FOVValue, 80.f, 90.f);
	GetFollowCamera()->SetFieldOfView(FOVValue);*/
}

void AJeremy::FireSlingshot()
{
	UE_LOG(LogTemp, Warning, TEXT("Slingshot Fired"));
	SpawnProjectile();
}

void AJeremy::SpawnProjectile()
{
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorForwardVector().Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(
		ProjectileClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
		);

	UE_LOG(LogTemp, Warning, TEXT("Projectile Spawned"));
}

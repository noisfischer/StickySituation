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

		EnhancedInputComponent->BindAction(AmmoSlotAction_1, ETriggerEvent::Triggered, this, &AJeremy::EquipAmmoSlot1);
		EnhancedInputComponent->BindAction(AmmoSlotAction_2, ETriggerEvent::Triggered, this, &AJeremy::EquipAmmoSlot2);
		EnhancedInputComponent->BindAction(AmmoSlotAction_3, ETriggerEvent::Triggered, this, &AJeremy::EquipAmmoSlot3);
		EnhancedInputComponent->BindAction(AmmoSlotAction_4, ETriggerEvent::Triggered, this, &AJeremy::EquipAmmoSlot4);
	}
}

void AJeremy::StartSlingshotPull()
{
	UE_LOG(LogTemp, Warning, TEXT("Slingshot Charge Begin"));

	PlaySound(SlingshotPull);
}

void AJeremy::ChargeSlingshot()
{
	if (ChargeValue < 1)
	{
		ChargeValue += 1 * GetWorld()->GetDeltaSeconds();	// CHARGE EVERY TICK //
		ChargeValue = FMath::Clamp(ChargeValue, 0, 1);
		UE_LOG(LogTemp, Warning, TEXT("Charge: %f"), ChargeValue);
	}

	else
	{
		if(!bChargeFull)
		{
			bChargeFull = true;
			PlaySound(SlingshotFullCharge);
			// Spawn Sparkle VFX on slingshot mesh
		}
	}
}

void AJeremy::FireSlingshot()
{
	UE_LOG(LogTemp, Warning, TEXT("Slingshot Fired"));
	
	SpawnProjectile(ChargeValue);	// ChargeValue USED AS DAMAGE/SPEED MULTIPLIER  //

	// RESET CHARGE VALUES //
	ChargeValue = 0;	
	if(bChargeFull)
		bChargeFull = false;

	PlaySound(SlingshotFired);
}

void AJeremy::SpawnProjectile(float Multiplier)
{
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = FollowCamera->GetComponentRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AProjectileBase* CurrentProjectile = GetWorld()->SpawnActor<AProjectileBase>
		(
		ProjectileClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
		);

	CurrentProjectile->SetCurrentSpeed(Multiplier);
	CurrentProjectile->SetBaseDamage(Multiplier);
	
	if(Multiplier < 0.25)
	{
		UE_LOG(LogTemp, Warning, TEXT("Very Low Speed & Damage"));
	}

	else if(Multiplier < 0.5)
	{
		UE_LOG(LogTemp, Warning, TEXT("Low Speed & Damage "));
	}

	else if(Multiplier < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Medium Speed & Damage"));
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Normal Speed & Damage"));
	}
}

void AJeremy::EquipAmmoSlot1()
{
	//if(ProjectileClass != )
	UE_LOG(LogTemp, Warning, TEXT("Ammo 1 selected"));
	
	PlaySound(SwitchAmmo);
	
	// ProjectileClass = 
}

void AJeremy::EquipAmmoSlot2()
{
	//if(ProjectileClass != )
	UE_LOG(LogTemp, Warning, TEXT("Ammo 2 selected"));

	PlaySound(SwitchAmmo);
	// ProjectileClass = 
}

void AJeremy::EquipAmmoSlot3()
{
	//if(ProjectileClass != )
	UE_LOG(LogTemp, Warning, TEXT("Ammo 3 selected"));

	PlaySound(SwitchAmmo);
	// ProjectileClass = 
}

void AJeremy::EquipAmmoSlot4()
{
	//if(ProjectileClass != )
	UE_LOG(LogTemp, Warning, TEXT("Ammo 4 selected"));

	PlaySound(SwitchAmmo);
	// ProjectileClass = 
}

void AJeremy::PlaySound(USoundBase* Sound)
{
	if(Sound != nullptr)
		UGameplayStatics::PlaySound2D
			(
			GetWorld(),
			Sound,
			1,
			1,
			0,
			nullptr,
			this,
			false
			);
}

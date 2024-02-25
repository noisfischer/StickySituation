// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Jeremy.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Projectiles/ProjectileBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/RedProjectile.h"
#include "Projectiles/GreenProjectile.h"
#include "Projectiles/BlueProjectile.h"
#include "Projectiles/YellowProjectile.h"


AJeremy::AJeremy()
{
	GrappleComponent = CreateDefaultSubobject<UGrappleComponent>(TEXT("Grapple"));
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

	// IF START PROJECTILE NOT SELECTED IN CHILD BP //
	if(StartProjectile != nullptr)
		CurrentProjectile = StartProjectile;
	else
		CurrentProjectile = RedProjectile;
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

		EnhancedInputComponent->BindAction(GrappleAction, ETriggerEvent::Triggered, this, &AJeremy::Grapple);

		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &AJeremy::Melee);

	}
}

void AJeremy::Melee()
{
	if (!bAttacking)
	{
		bAttacking = true;
		PlayAnimMontage(MeleeMontage, MeleeSpeed, FName("None"));
	}
}

void AJeremy::OnWeaponCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Super::OnWeaponCollisionOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if(OtherActor->ActorHasTag("enemy"))
	{
		UE_LOG(LogTemp, Warning, TEXT("OVERLAP"));
		Execute_DamageEnemy(OtherActor, MeleeDamage);
	}
		
}

void AJeremy::StartSlingshotPull()
{
	UE_LOG(LogTemp, Warning, TEXT("Slingshot Charge Begin"));

	bCharging = true;

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

	if (ChargeValue < .15)
		ChargeValue = .15;
	
	SpawnProjectile(ChargeValue);	// ChargeValue USED AS DAMAGE/SPEED MULTIPLIER  //
	PlaySound(SlingshotFired);

	// RESET CHARGE VALUES //
	ChargeValue = 0;	
	if(bChargeFull)
		bChargeFull = false;

	bCharging = false;
	
}

void AJeremy::SpawnProjectile(float Multiplier)
{
	FVector SpawnLocation = GetMesh()->GetBoneLocation("Head") + (FollowCamera->GetForwardVector() * 250);
	FRotator SpawnRotation = FollowCamera->GetComponentRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>
		(
		CurrentProjectile,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
		);

	Projectile->SetCurrentSpeed(Multiplier);
	Projectile->SetBaseDamage(Multiplier);
	
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

void AJeremy::EquipAmmo(EAmmoType AmmoType)
{
	if(bCharging)
		return;

	TSubclassOf<AProjectileBase> DesiredProjectile = nullptr;
	FString AmmoName;

	switch(AmmoType)
	{
		case EAmmoType::Red:
			DesiredProjectile = RedProjectile;
			AmmoName = "Red";
			break;

		case EAmmoType::Green:
			DesiredProjectile = GreenProjectile;
			AmmoName = "Green";
			break;

		case EAmmoType::Blue:
			DesiredProjectile = BlueProjectile;
			AmmoName = "Blue";
			break;

		case EAmmoType::Yellow:
			DesiredProjectile = YellowProjectile;
			AmmoName = "Yellow";
			break;
	}

	if(CurrentProjectile != DesiredProjectile)
	{
		CurrentProjectile = DesiredProjectile;
		PlaySound(SwitchAmmo);
		UE_LOG(LogTemp, Warning, TEXT("%s Ammo selected"), *AmmoName);
	}
}

void AJeremy::EquipAmmoSlot1()
{
	EquipAmmo(EAmmoType::Red);
}

void AJeremy::EquipAmmoSlot2()
{
	EquipAmmo(EAmmoType::Green);
}

void AJeremy::EquipAmmoSlot3()
{
	EquipAmmo(EAmmoType::Blue);
}

void AJeremy::EquipAmmoSlot4()
{
	EquipAmmo(EAmmoType::Yellow);
}


void AJeremy::Grapple()
{

	check(GrappleComponent);
	
	if(GrappleComponent)
	{
		FVector StartLocation = GetMesh()->GetBoneLocation("Head") + (FollowCamera->GetForwardVector() * 250);
		FRotator Rotation = FollowCamera->GetComponentRotation();
		
		FVector CameraDirection = FollowCamera->GetForwardVector();
		FVector EndLocation = StartLocation + (CameraDirection * GrappleComponent->GrappleRange);
		
		GrappleComponent->ActivateGrapple(StartLocation, Rotation, EndLocation);
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GrappleComponent not attached"));
	}
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

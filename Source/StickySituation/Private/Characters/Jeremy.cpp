// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Jeremy.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameMacros.h"
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

	ensure(GrappleComponent);
	ensure(!ProjectileMap.Num() == 0);
	ensure(!AmmoCountMap.Num() == 0);

	InitializeAmmo();
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

void AJeremy::InitializeAmmo()
{
	EQUIP_AMMO_TYPE(CurrentAmmoType);	// from GameMacros.h

	AmmoCountMap.Add(EAmmoType::Red, 1);
	AmmoCountMap.Add(EAmmoType::Green, 1);
	AmmoCountMap.Add(EAmmoType::Blue, 1);
	AmmoCountMap.Add(EAmmoType::Yellow, 1);
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
	if(*AmmoCountMap.Find(CurrentAmmoType) != 0)
	{
		bCharging = true;
		PlaySound(SlingshotPull);
	}
	
}

void AJeremy::ChargeSlingshot()
{
	if (bCharging && ChargeValue < 1)
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
	if(bCharging)
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
}


void AJeremy::EquipAmmoSlot1()
{
	if(CurrentAmmoType != EAmmoType::Red && !bCharging)
	{
		EQUIP_AMMO_TYPE(EAmmoType::Red);
		CurrentAmmoType = EAmmoType::Red;
	}
}

void AJeremy::EquipAmmoSlot2()
{
	if(CurrentAmmoType != EAmmoType::Green && !bCharging)
	{
		EQUIP_AMMO_TYPE(EAmmoType::Green);
		CurrentAmmoType = EAmmoType::Green;
	}
}

void AJeremy::EquipAmmoSlot3()
{
	if(CurrentAmmoType != EAmmoType::Blue && !bCharging)
	{
		EQUIP_AMMO_TYPE(EAmmoType::Blue);
		CurrentAmmoType = EAmmoType::Blue;
	}
}

void AJeremy::EquipAmmoSlot4()
{
	if(CurrentAmmoType != EAmmoType::Yellow && !bCharging)
	{
		EQUIP_AMMO_TYPE(EAmmoType::Yellow);
		CurrentAmmoType = EAmmoType::Yellow;
	}
}


void AJeremy::Grapple()
{

	check(GrappleComponent);
	
	if(GrappleComponent && bGrappleAvailable)
	{
		FVector StartLocation = GetMesh()->GetBoneLocation("Head") + (FollowCamera->GetForwardVector() * 250);
		FRotator Rotation = FollowCamera->GetComponentRotation();
		
		FVector CameraDirection = FollowCamera->GetForwardVector();
		FVector EndLocation = StartLocation + (CameraDirection * GrappleComponent->GrappleRange);
		
		GrappleComponent->ActivateGrapple(StartLocation, Rotation, EndLocation);

		bGrappleAvailable = false;

		GetWorld()->GetTimerManager().SetTimer(GrappleRefreshTimer, this, &AJeremy::GrappleRefresh, RefreshGrappleTime, false);
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Grapple refreshing"));
		PlaySound(GrappleUnavailable);
	}
}

void AJeremy::GrappleRefresh()
{
	bGrappleAvailable = true;;
}


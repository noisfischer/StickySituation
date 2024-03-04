// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Jeremy.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameMacros.h"
#include "Projectiles/ProjectileBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/RedProjectile.h"
#include "Projectiles/GreenProjectile.h"
#include "Projectiles/BlueProjectile.h"
#include "Projectiles/YellowProjectile.h"


AJeremy::AJeremy()
{
	GrappleComponent = CreateDefaultSubobject<UGrappleComponent>(TEXT("Grapple"));

	//	CHARACTER SKILL TREE //
	FCharacterSkills MovementSpeedIncrease;
	MovementSpeedIncrease.Name = TEXT("MovementSpeedIncrease");
	MovementSpeedIncrease.Value = 0.5f;
	MovementSpeedIncrease.Description = FString::Printf(TEXT("Movement speed increase by: %.2f"), MovementSpeedIncrease.Value);
	
	FCharacterSkills MaxHealthIncrease;
	MaxHealthIncrease.Name = TEXT("MaxHealthIncrease");
	MaxHealthIncrease.Value = 0.5f;
	MaxHealthIncrease.Description = FString::Printf(TEXT("Max Health increase by: %.2f"), MaxHealthIncrease.Value);

	FCharacterSkills MeleeDamageIncrease;
	MeleeDamageIncrease.Name = TEXT("MeleeDamageIncrease");
	MeleeDamageIncrease.Value = 0.15f;
	MeleeDamageIncrease.Description = FString::Printf(TEXT("Melee damage increase by: %.2f"), MeleeDamageIncrease.Value);

	FCharacterSkills ProjectileDamageIncrease;
	ProjectileDamageIncrease.Name = TEXT("ProjectileDamageIncrease");
	ProjectileDamageIncrease.Value = 0.15f;
	ProjectileDamageIncrease.Description = FString::Printf(TEXT("Projectile damage increase by: %.2f"), ProjectileDamageIncrease.Value);

	CharacterSkillTreeComponent->CharacterSkillMap.Add(MovementSpeedIncrease.Name, MovementSpeedIncrease);
	CharacterSkillTreeComponent->CharacterSkillMap.Add(MaxHealthIncrease.Name, MaxHealthIncrease);
	CharacterSkillTreeComponent->CharacterSkillMap.Add(MeleeDamageIncrease.Name, MeleeDamageIncrease);
	CharacterSkillTreeComponent->CharacterSkillMap.Add(ProjectileDamageIncrease.Name, ProjectileDamageIncrease);

	///////////////////////////////////////////////////////
	
	CharacterIdentifier = TEXT("Jeremy");	// for save slot
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
	// check if any upgrades/skills equipped
		// i.e. ProjectileMap.Find(EAmmoType::Red)->Damage += 1;
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
	if(ProjectileMap.Find(CurrentAmmoName)->AmmoCount != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Slingshot Charge Begins"));
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
		if(ChargeValue == 1)
			UE_LOG(LogTemp, Warning, TEXT("Fully Charged"));
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
		if (ChargeValue < .15)
			ChargeValue = .15;

		float ProjectileDamage = ProjectileDamageMultiplier * ChargeValue;
	
		SpawnProjectile(ProjectileDamage, ChargeValue);
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
	if(CurrentAmmoName != EAmmoType::Red && !bCharging)
		CurrentAmmoName = EAmmoType::Red;
}

void AJeremy::EquipAmmoSlot2()
{
	if(CurrentAmmoName != EAmmoType::Green && !bCharging)
		CurrentAmmoName = EAmmoType::Green;
}

void AJeremy::EquipAmmoSlot3()
{
	if(CurrentAmmoName != EAmmoType::Blue && !bCharging)
		CurrentAmmoName = EAmmoType::Blue;
}

void AJeremy::EquipAmmoSlot4()
{
	if(CurrentAmmoName != EAmmoType::Yellow && !bCharging)
		CurrentAmmoName = EAmmoType::Yellow;
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
	bGrappleAvailable = true;
}


void AJeremy::InitializeUnlockedSkills()
{
	Super::InitializeUnlockedSkills();	// If activeskills < 0, return

	for(auto skill : ActiveSkills)
	{
		if(skill == "MovementSpeedIncrease")
		{
			GetCharacterMovement()->MaxWalkSpeed += GetCharacterMovement()->MaxWalkSpeed * GET_SKILL_VALUE(skill);
			UE_LOG(LogTemp, Warning, TEXT("MaxSpeed: %f"), GetCharacterMovement()->MaxWalkSpeed);
		}
		else if (skill == "MaxHealthIncrease")
		{
			MaxHealth += MaxHealth * GET_SKILL_VALUE(skill);
			CurrentHealth = MaxHealth;
			UE_LOG(LogTemp, Warning, TEXT("MaxHealth: %f"), MaxHealth);
		}
		else if (skill == "MeleeDamageIncrease")
		{
			MeleeDamage += MeleeDamage * MeleeDamageMultiplier;
			UE_LOG(LogTemp, Warning, TEXT("Melee Damage: %f"), MeleeDamage);
		}
		else if (skill == "ProjectileDamageIncrease")
		{
			ProjectileDamageMultiplier += ProjectileDamageMultiplier * GET_SKILL_VALUE(skill);
			UE_LOG(LogTemp, Warning, TEXT("Projectile Damage Multiplier: %f"), ProjectileDamageMultiplier);
		}	
	}
}

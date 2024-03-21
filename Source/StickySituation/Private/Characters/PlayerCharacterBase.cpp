#include "Characters/PlayerCharacterBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameMacros.h"
#include "InputActionValue.h"
#include "NiagaraFunctionLibrary.h"
#include "SPinValueInspector.h"
#include "GameModes/StickySituationGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/RedProjectile.h"
#include "Projectiles/GreenProjectile.h"
#include "Projectiles/BlueProjectile.h"
#include "Projectiles/YellowProjectile.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);


// CONSTRUCTOR //
APlayerCharacterBase::APlayerCharacterBase()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// CAMERA ROTATION PROPERTIES //
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// CHARACTER MOVEMENT PROPERTIES //
	GetCharacterMovement()->bUseControllerDesiredRotation = true; // PLAYER ALWAYS FACES FORWARD
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->MaxAcceleration = 1500.f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.f;
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

	// SETUP CAMERA BOOM //
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// SETUP FOLLOW CAMERA //
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// SETUP MESH ATTACHMENTS //
	Pack_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Backpack"));
	if(Backpack_SocketName.IsValid())
		Pack_Mesh->SetupAttachment(GetMesh(), "BackpackSocket");

	ProjectileWeapon_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ProjectileWeapon"));
	if(ProjectileWeapon_SocketName.IsValid())
		ProjectileWeapon_Mesh->SetupAttachment(GetMesh(), ProjectileWeapon_SocketName);

	MeleeWeapon_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeleeWeapon"));
	if(MeleeWeapon_SocketName.IsValid())
		MeleeWeapon_Mesh->SetupAttachment(GetMesh(), MeleeWeapon_SocketName);

	CharacterSkillTreeComponent = CreateDefaultSubobject<USkillTreeComponent>(TEXT("CharacterSkillTree"));

	MeleeWeapon_Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeWeaponCollision"));
	MeleeWeapon_Collision->SetupAttachment(MeleeWeapon_Mesh);
	MeleeWeapon_Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CharacterSkillTreeComponent->OnSkillUnlocked.AddDynamic(this, &APlayerCharacterBase::HandleSkillUnlocked);
	CharacterSkillTreeComponent->OnSkillActivated.AddDynamic(this, &APlayerCharacterBase::HandleSkillActivated);
	CharacterSkillTreeComponent->OnSkillDeactivated.AddDynamic(this, &APlayerCharacterBase::HandleSkillDeactivated);

	CurrentState = EPlayerState::Neutral;
	
	ProjectileDataTablePath = TEXT("/Game/DataTables/ProjectileData");
}


void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	LoadCharacterData();
	LoadCurrency();
	LoadProjectileData();

	AStickySituationGameMode* CurrentGameMode = Cast<AStickySituationGameMode>(UGameplayStatics::GetGameMode(this));
	if (CurrentGameMode)
	{
		
	}

	// MELEE ANIMATION SETUP //
	MeleeWeapon_Collision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacterBase::OnWeaponCollisionOverlap);
	AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance != nullptr)
		AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacterBase::OnMontageFinished);
}


void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacterBase::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacterBase::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacterBase::PerformCollectAmmo);
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &APlayerCharacterBase::Melee);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APlayerCharacterBase::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacterBase::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacterBase::PerformCollectAmmo()
{
	if(CurrentState == EPlayerState::Hub)
		return;
	
	if(CollectAmmoMontage != nullptr)
	{
		switch(CurrentState)
		{
		case EPlayerState::Neutral:
			PlayAnimMontage(CollectAmmoMontage, 1.f, "None");
			break;
		default:
			break;
		}
	}
}

void APlayerCharacterBase::CollectAmmo()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	FVector Location = GetActorLocation() + GetActorForwardVector() * 75;
	float Radius = 50.f;

	GetWorld()->SweepMultiByChannel(
		HitResults,
		Location,
		Location,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(Radius),
		QueryParams
		);

	DrawDebugSphere(
		GetWorld(),
		Location,
		Radius,
		50,
		FColor::Red,
		false,
		1.f,
		0
		);

	for(auto Hit : HitResults)
	{
		if(Hit.GetActor()->ActorHasTag("stunned"))
		{
			// SPAWN NIAGARA SYSTEM //
			/*UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ParticleSystem,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
			);*/

			// SPAWN SOUND //
			/*UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				CollectAmmoSound,
				Hit.Location,
				Hit.GetActor()->GetActorRotation(),
				1.f,
				1.f,
				0.f,
				nullptr,
				nullptr,
				nullptr,
				nullptr
				);*/

			FName EnemyColorTag = Hit.GetActor()->Tags[1];	// assign a color tag in BP i.e. ("Blue")
			Hit.GetActor()->Destroy();
			IncreaseAmmo(EnemyColorTag);
		}
	}
}

void APlayerCharacterBase::IncreaseAmmo(const FName& ColorTag)
{
	for(auto Projectile : ProjectileMap)
	{
		if(ColorTag == ProjectileMap.Find(Projectile.Key)->Name)
		{
			int32& AmmoIncrease = ProjectileMap.Find(Projectile.Key)->AmmoCount;
			AmmoIncrease++;
			UE_LOG(LogTemp, Warning, TEXT("%s: %i"), *ColorTag.ToString(), AmmoIncrease);
			break;
		}
	}
}



// INTERFACE EVENTS //
void APlayerCharacterBase::DamagePlayer_Implementation(const float Damage)
{
	CurrentHealth -= Damage;
	UE_LOG(LogTemp, Warning, TEXT("Player Current Health: %f"), CurrentHealth);
}

void APlayerCharacterBase::HealPlayer_Implementation(const float Heal)
{
	CurrentHealth += Heal;
	UE_LOG(LogTemp, Warning, TEXT("Current Health: %f"), CurrentHealth);
}


void APlayerCharacterBase::ActivateMelee()
{
	MeleeWeapon_Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APlayerCharacterBase::DeactivateMelee()
{
	MeleeWeapon_Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerCharacterBase::Melee()
{
	if (MeleeMontage != nullptr)
	{
		switch(CurrentState)
		{
		case EPlayerState::Neutral:
			PlayAnimMontage(MeleeMontage, MeleeSpeed, FName("None"));
			break;
		default:
			break;
		}
	}
}

void APlayerCharacterBase::OnWeaponCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("enemy"))
	{
		float RandomNum = FMath::RandRange(0, 1);
		if(RandomNum <= StunChance)		// Default 20% chance (no skill active)
		{
			Execute_StunEnemy(OtherActor, StunTime);	// Default 5 seconds (no skill active)
			UE_LOG(LogTemp, Warning, TEXT("Stun Time: %f"), StunTime);
			UE_LOG(LogTemp, Warning, TEXT("Stun Chance: %f"), StunChance);
		}
		else
			Execute_DamageEnemy(OtherActor, MeleeDamage);
	}
}


void APlayerCharacterBase::SpawnProjectile(float DamageMultiplier, float SpeedMultiplier)
{
	FVector SpawnLocation = GetMesh()->GetBoneLocation("Head") + (FollowCamera->GetForwardVector() * 250);
	FRotator SpawnRotation = FollowCamera->GetComponentRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	TSubclassOf<AProjectileBase> SpawnProjectile = ProjectileMap.Find(CurrentAmmoName)->ProjectileClass;

	
	if(*SpawnProjectile)
	{
		AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>
			(
			SpawnProjectile,
			SpawnLocation,
			SpawnRotation,
			SpawnParams
			);
		
		Projectile->SetCurrentSpeed(ProjectileMap.Find(CurrentAmmoName)->Speed * SpeedMultiplier);
		Projectile->SetDamage(ProjectileMap.Find(CurrentAmmoName)->Damage * DamageMultiplier);
		if(Projectile->IsA(ABlueProjectile::StaticClass()))
		{
			Projectile->StunTime += Projectile->StunTime * StunTimeMultiplier;
			Projectile->StunChance += Projectile->StunChance * StunChanceMultiplier;
		}
		
		ProjectileMap.Find(CurrentAmmoName)->AmmoCount--;
	}
}

float APlayerCharacterBase::PlayAnimMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	float MontageLength = 0.f;
	if(AnimMontage && AnimInstance)
	{
		MontageLength = AnimInstance->Montage_Play(AnimMontage, InPlayRate);
		CurrentState = EPlayerState::Busy;
	}
	return MontageLength;
}

void APlayerCharacterBase::OnMontageFinished(UAnimMontage* Montage, bool bMontageInterrupted)
{
	if(CurrentState != EPlayerState::CanInteract)	// For when player is near an interactable
		CurrentState = EPlayerState::Neutral;
}

void APlayerCharacterBase::PlaySound(USoundBase* Sound)
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


void APlayerCharacterBase::HandleSkillUnlocked(FString SkillName)
{
	SaveCharacterData();	// saves current CharacterSkillMap values (unlocked)
}

void APlayerCharacterBase::HandleSkillActivated(FString SkillName)
{
	ActiveSkills.Add(SkillName);
	SaveCharacterData();
	UE_LOG(LogTemp, Warning, TEXT("Skill activated: %s"), *SkillName);
}

void APlayerCharacterBase::HandleSkillDeactivated(FString SkillName)
{
	ActiveSkills.Remove(SkillName);
	SaveCharacterData();
	UE_LOG(LogTemp, Warning, TEXT("Skill deactivated: %s"), *SkillName);
}

void APlayerCharacterBase::SaveCharacterData()
{
	USaveCharacterData* SaveGameInstance = Cast<USaveCharacterData>(UGameplayStatics::CreateSaveGameObject(USaveCharacterData::StaticClass()));

	// SAVE CURRENT SKILL DATA //
	SaveGameInstance->SavedSkills = ActiveSkills;
	SaveGameInstance->SavedSkillTreeState = CharacterSkillTreeComponent->CharacterSkillMap;

	// SAVE CURRENT XP & LEVEL DATA //
	SaveGameInstance->CharacterLevel = this->CharacterLevel;
	SaveGameInstance->CharacterXP = this->CharacterXP;
	
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, CharacterIdentifier, 0);
	UE_LOG(LogTemp, Warning, TEXT("Saved Character Data"));
}

void APlayerCharacterBase::LoadCharacterData()
{
	if (UGameplayStatics::DoesSaveGameExist(CharacterIdentifier, 0))
	{
		USaveCharacterData* LoadGameInstance = Cast<USaveCharacterData>(UGameplayStatics::LoadGameFromSlot(CharacterIdentifier, 0));
		if (LoadGameInstance != nullptr)
		{
			// LOAD & INIT CHARACTER SKILL INFORMATION //
			ActiveSkills = LoadGameInstance->SavedSkills;
			CharacterSkillTreeComponent->CharacterSkillMap = LoadGameInstance->SavedSkillTreeState;
			InitializeUnlockedSkills();

			// LOAD CHARACTER XP AND LEVEL DATA //
			CharacterLevel = LoadGameInstance->CharacterLevel;
			CharacterXP = LoadGameInstance->CharacterXP;
		}
		else
		{
			UDataTable* DataTable = LoadObject<UDataTable>(nullptr, *SkillsDataTablePath);
			if(DataTable)
				InitializeCharacterSkillsFromDataTable(DataTable);
			else
				UE_LOG(LogTemp, Error, TEXT("Failed to load SkillTree DataTable."));
		}
	}
	else
	{
		UDataTable* DataTable = LoadObject<UDataTable>(nullptr, *SkillsDataTablePath);
		if(DataTable)
			InitializeCharacterSkillsFromDataTable(DataTable);
		else
			UE_LOG(LogTemp, Error, TEXT("Failed to load SkillTree DataTable."));
	}

	UE_LOG(LogTemp, Warning, TEXT("Current Level: %f"), CharacterLevel);
	UE_LOG(LogTemp, Warning, TEXT("Current XP: %f"), CharacterXP);
}

void APlayerCharacterBase::InitializeUnlockedSkills()
{
	if(ActiveSkills.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Active Skills"));
		return;
	}
	
	// OTHER LOGIC IN OVERRIDDEN FUNCTIONS IN DERIVED CLASSES
}

bool APlayerCharacterBase::InitializeCharacterSkillsFromDataTable(UDataTable* DataTable)
{
	if (!DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable is not valid."));
		return false;
	}

	static const FString ContextString(TEXT("Character Skills Initialization"));
	for (const auto& Row : DataTable->GetRowMap())
	{
		FCharacterSkills* SkillData = (FCharacterSkills*)Row.Value;

		if (SkillData && CharacterSkillTreeComponent)
		{
			CharacterSkillTreeComponent->CharacterSkillMap.Add(SkillData->Name, *SkillData);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Character skills have been initialized from DataTable."));
	return true;
}

void APlayerCharacterBase::SaveCurrency()
{
	USaveCharacterData* SaveGameInstance = Cast<USaveCharacterData>(UGameplayStatics::CreateSaveGameObject(USaveCharacterData::StaticClass()));
	
	SaveGameInstance->Currency = this->Currency;
	
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("PlayerCurrency"), 0);
	UE_LOG(LogTemp, Warning, TEXT("Saved Currency Data"));
}

void APlayerCharacterBase::LoadCurrency()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerCurrency"), 0))
	{
		USaveCharacterData* LoadGameInstance = Cast<USaveCharacterData>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerCurrency"), 0));
		if (LoadGameInstance != nullptr)
			Currency = LoadGameInstance->Currency;
	}

	UE_LOG(LogTemp, Warning, TEXT("Currency: %f"), Currency);
}

void APlayerCharacterBase::LoadProjectileData()
{
	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, *ProjectileDataTablePath);
	if (!DataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile DataTable not found."));
		return;
	}

	static const FString ContextString(TEXT("Projectile Data Loading"));
	ProjectileMap.Empty();
	for (const auto& Row : DataTable->GetRowMap())
	{
		FName RowName = Row.Key;
		FProjectileData* RowData = (FProjectileData*)Row.Value;

		if (RowData)
		{
			FString RowNameStr = RowName.ToString();
			ProjectileMap.Add(RowNameStr, *RowData);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Projectile data loaded successfully."));
}

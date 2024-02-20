// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/PlayerCharacterBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

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

	MeleeWeapon_Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeWeaponCollision"));
	MeleeWeapon_Collision->SetupAttachment(MeleeWeapon_Mesh);
	MeleeWeapon_Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void APlayerCharacterBase::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}


// INTERFACE EVENTS //

void APlayerCharacterBase::DamagePlayer_Implementation(float Damage)
{
	CurrentHealth -= Damage;
	UE_LOG(LogTemp, Warning, TEXT("Player Current Health: %f"), CurrentHealth);
}

void APlayerCharacterBase::HealPlayer_Implementation(float Heal)
{
	CurrentHealth += Heal;
	UE_LOG(LogTemp, Warning, TEXT("Current Health: %f"), CurrentHealth);
}



// INPUT EVENTS //

void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacterBase::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacterBase::Look);
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
// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PlayerCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UCapsuleComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class APlayerCharacterBase : public ACharacter
{
	GENERATED_BODY()


	// COMPONENT DECLARATIONS TO BE INITIALIZED IN CONSTRUCTOR //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Backpack_Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ProjectileWeapon_Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeleeWeapon_Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* MeleeWeapon_Collision;


	// ENHANCED INPUT MAPPINGS TO BE SET IN CHILD BP //
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	
public:
	// CONSTRUCTOR
	APlayerCharacterBase();


	// HEALTH ATTRIBUTES //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1 - HEALTH")
	float MaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1 - HEALTH")
	float CurrentHealth = MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1 - HEALTH")
	bool Dead = false;


	// AMMO //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2 - AMMO")
	float RedAmmo = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2 - AMMO")
	float GreenAmmo = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2 - AMMO")
	float BlueAmmo = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2 - AMMO")
	float YellowAmmo = 1;
	

	// SOCKET NAMES FOR ATTACHMENT TO SKELETAL MESH //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3 - SOCKET NAMES")
	FName Backpack_SocketName = "BackpackSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3 - SOCKET NAMES")
	FName MeleeWeapon_SocketName = "LeftHandSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3 - SOCKET NAMES")
	FName ProjectileWeapon_SocketName = "RightHandSocket";
	

protected:

	// ENHANCED INPUT FUNCTION BINDINGS
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
			

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};


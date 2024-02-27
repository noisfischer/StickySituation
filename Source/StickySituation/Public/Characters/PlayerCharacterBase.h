// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEnums.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Interfaces/CharacterInterface.h"
#include "PlayerCharacterBase.generated.h"

class AYellowProjectile;
class ABlueProjectile;
class AGreenProjectile;
class ARedProjectile;
class AProjectileBase;
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
class APlayerCharacterBase : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

	// ENHANCED INPUT MAPPINGS TO BE SET IN CHILD BP (private, by default) //
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	
public:
	// CONSTRUCTOR DECLARATION //
	APlayerCharacterBase();

	// COMPONENT DECLARATIONS TO BE INITIALIZED IN CONSTRUCTOR //
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMeshComponent* Pack_Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* ProjectileWeapon_Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* MeleeWeapon_Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	UCapsuleComponent* MeleeWeapon_Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation")
	UAnimInstance* AnimInstance = nullptr;

	virtual void ActivateMelee();
	virtual void DeactivateMelee();

	// HEALTH ATTRIBUTES //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1 - HEALTH")
	float MaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1 - HEALTH")
	float CurrentHealth = MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1 - HEALTH")
	bool Dead = false;

	// POPULATE THESE IN YOUR CHILD BPs! //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2 - AMMO")
	TMap<EAmmoType, TSubclassOf<AProjectileBase>> ProjectileMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2 - AMMO")
	TMap<EAmmoType, int32> AmmoCountMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2 - AMMO")
	EAmmoType CurrentAmmoType = EAmmoType::Red;

	// USED TO SET THE CURRENT AMMO TYPE //
	UPROPERTY(EditDefaultsOnly, Category = "2 - AMMO")
	TSubclassOf<AProjectileBase> CurrentProjectile = nullptr;

	// AMMO //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2 - AMMO")
	int32 RedAmmo = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2 - AMMO")
	int32 GreenAmmo = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2 - AMMO")
	int32 BlueAmmo = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2 - AMMO")
	int32 YellowAmmo = 1;
	

	// SOCKET NAMES FOR ATTACHMENT TO SKELETAL MESH //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3 - SOCKET NAMES")
	FName Backpack_SocketName = "BackpackSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3 - SOCKET NAMES")
	FName MeleeWeapon_SocketName = "LeftHandSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3 - SOCKET NAMES")
	FName ProjectileWeapon_SocketName = "RightHandSocket";


	// INTERFACE EVENTS //
	virtual void DamagePlayer_Implementation(float Damage) override;

	virtual void HealPlayer_Implementation(float Heal) override;

	UFUNCTION()
	virtual void OnMontageFinished(UAnimMontage* Montage, bool bMontageInterrupted);
	
	UFUNCTION()
	virtual void OnWeaponCollisionOverlap
		(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);

	UFUNCTION(BlueprintCallable)
	virtual void PlaySound(USoundBase* Sound);
	
protected:
	virtual float PlayAnimMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName) override;

	virtual void UseAmmo(EAmmoType CurrentAmmo);
	
	void SpawnProjectile(float Multiplier);
	
	bool bAttacking = false;

	// ENHANCED INPUT FUNCTION BINDINGS
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};


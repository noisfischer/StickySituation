// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEnums.h"
#include "GameStructs.h"
#include "SaveCharacterData.h"
#include "Components/SkillTreeComponent.h"
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
	
	UFUNCTION()
	virtual void SaveCharacterSkills();
	UFUNCTION()
	virtual void LoadCharacterSkills();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Save")
	FString CharacterIdentifier;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	USkillTreeComponent* CharacterSkillTreeComponent;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DAMAGE")
	float ProjectileDamageMultiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DAMAGE")
	float MeleeDamageMultiplier = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2 - AMMO")
	EAmmoType CurrentAmmoName = EAmmoType::Red;

	// POPULATE THIS IN YOUR CHILD BPs! //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2 - AMMO")
	TMap<EAmmoType, FProjectileData> ProjectileMap;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	TArray<FString> ActiveSkills;
	UFUNCTION(BlueprintCallable)
	virtual void InitializeUnlockedSkills();
	UFUNCTION(BlueprintCallable)
	virtual void HandleSkillUnlocked(FString SkillName);
	UFUNCTION(BlueprintCallable)
	virtual void HandleSkillActivated(FString SkillName);
	UFUNCTION(BlueprintCallable)
	virtual void HandleSkillDeactivated(FString SkillName);
	
protected:
	virtual float PlayAnimMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName) override;
	
	void SpawnProjectile(float DamageMultiplier, float SpeedMultiplier);
	
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


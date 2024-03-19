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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	
public:
	// CONSTRUCTOR DECLARATION //
	APlayerCharacterBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	EPlayerState CurrentState;
	
	UFUNCTION(BlueprintCallable)
	virtual void SaveCharacterData();
	UFUNCTION(BlueprintCallable)
	virtual void LoadCharacterData();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Save")
	FString CharacterIdentifier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Save")
	FString SkillsDataTablePath;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Save")
	FString ProjectileDataTablePath;
	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimMontage* CollectAmmoMontage = nullptr;
	UFUNCTION(BlueprintCallable)
	virtual void CollectAmmo();
	UFUNCTION(BlueprintCallable)
	virtual void IncreaseAmmo(FName ColorTag);
	
	virtual void ActivateMelee();
	virtual void DeactivateMelee();

	// HEALTH ATTRIBUTES //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1 - HEALTH")
	float MaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1 - HEALTH")
	float CurrentHealth = MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1 - HEALTH")
	bool Dead = false;

	// PROJECTILE MULTIPLIERS //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DAMAGE")
	float ProjectileDamageMultiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DAMAGE")
	float MeleeDamageMultiplier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="STUN")
	float StunTime = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="STUN")
	float StunTimeMultiplier = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="STUN")
	float StunChance = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="STUN")
	float StunChanceMultiplier = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="XP")
	float CharacterXP = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="XP")
	float CharacterLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Currency")
	float Currency = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2 - AMMO")
	FString CurrentAmmoName = TEXT("Red");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2 - AMMO")
	TMap<FString, FProjectileData> ProjectileMap;
	
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

	bool InitializeCharacterSkillsFromDataTable(UDataTable* DataTable);

	UFUNCTION(BlueprintCallable)
	void SaveCurrency();
	UFUNCTION(BlueprintCallable)
	void LoadCurrency();
	
	void LoadProjectileData();
	
	void SpawnProjectile(float DamageMultiplier, float SpeedMultiplier);
	
	bool bAttacking = false;

	// ENHANCED INPUT FUNCTION BINDINGS
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	virtual void OnInteractActionTriggered();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};


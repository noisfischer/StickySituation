#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "Components/ActorComponent.h"
#include "SkillTreeComponent.generated.h"

// Can be used to update all subscribers to this delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillUnlocked, FString, SkillName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillRetracted, FString, SkillName);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STICKYSITUATION_API USkillTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillTreeComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SkillTree")
	TMap<FString, FCharacterSkills> CharacterSkillMap;

	// Delegate instance for skill unlock notification
	UPROPERTY(BlueprintAssignable, Category="Skills")
	FOnSkillUnlocked OnSkillUnlocked;
	UPROPERTY(BlueprintAssignable, Category="Skills")
	FOnSkillUnlocked OnSkillRetracted;

	UFUNCTION(BlueprintCallable)
	void UnlockSkill(const FString& SkillName);
	UFUNCTION(BlueprintCallable)
	void RetractSkill (const FString& SkillName);
	
	
protected:
	virtual void BeginPlay() override;

public:	
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

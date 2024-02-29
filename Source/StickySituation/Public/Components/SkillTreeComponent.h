#pragma once

#include "CoreMinimal.h"
#include "GameEnums.h"
#include "GameStructs.h"
#include "Components/ActorComponent.h"
#include "SkillTreeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STICKYSITUATION_API USkillTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillTreeComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SkillTree")
	TMap<FString, FCharacterSkills> CharacterSkillMap;

	UFUNCTION(BlueprintCallable)
	void UnlockSkill(const FString& SkillName);
	
protected:
	virtual void BeginPlay() override;

public:	
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

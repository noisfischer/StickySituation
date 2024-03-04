#include "Components/SkillTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SaveCharacterData.h"

USkillTreeComponent::USkillTreeComponent()
{
	// PrimaryComponentTick.bCanEverTick = true;
}


void USkillTreeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USkillTreeComponent::UnlockSkill(const FString& SkillName)
{
	
	FCharacterSkills* Skill = CharacterSkillMap.Find(SkillName);
	
	if(Skill && !Skill->bIsUnlocked)
	{
		Skill->bIsUnlocked = true;
		OnSkillUnlocked.Broadcast(SkillName); // bound in base character constructor (data is saved)
	}
}

int32 USkillTreeComponent::GetNumActiveSkills() const
{
	int32 ActiveSkillsCount = 0;
	
	for(const auto& SkillEntry : CharacterSkillMap)
	{
		if(SkillEntry.Value.bIsActive)
			ActiveSkillsCount++;
	}
	return ActiveSkillsCount;
}

void USkillTreeComponent::ActivateSkill(const FString& SkillName)
{
	if(GetNumActiveSkills() < 3)
	{
		FCharacterSkills* Skill = CharacterSkillMap.Find(SkillName);
		if(!Skill->bIsActive && Skill->bIsUnlocked)
		{
			Skill->bIsActive = true;
			OnSkillActivated.Broadcast(SkillName); // bound in base character constructor (data is saved)
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Too many skills active"));
	}
}

void USkillTreeComponent::DeactivateSkill(const FString& SkillName)
{
	FCharacterSkills* Skill = CharacterSkillMap.Find(SkillName);
	if(Skill->bIsActive && Skill->bIsUnlocked)
	{
		Skill->bIsActive = false;
		OnSkillDeactivated.Broadcast(SkillName); // bound in base character constructor (data is saved)
	}
}




/*
void USkillTreeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
*/


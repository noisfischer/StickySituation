#include "Components/SkillTreeComponent.h"

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
		OnSkillUnlocked.Broadcast(SkillName); // calls HandleSkillUnlocked in PlayerCharacterBase
	}
}

void USkillTreeComponent::RetractSkill(const FString& SkillName)
{
	FCharacterSkills* Skill = CharacterSkillMap.Find(SkillName);
	
	if(Skill && Skill->bIsUnlocked)
	{
		Skill->bIsUnlocked = false;
		OnSkillRetracted.Broadcast(SkillName);	// calls HandleSkillRetracted in PlayerCharacterBase
	}
}


/*
void USkillTreeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
*/


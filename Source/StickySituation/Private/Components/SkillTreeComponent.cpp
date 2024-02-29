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
	if(FCharacterSkills* Skill = CharacterSkillMap.Find(SkillName))	// Attempt to find SkillName
		UE_LOG(LogTemp, Warning, TEXT("Search Successful"));
}


/*
void USkillTreeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
*/


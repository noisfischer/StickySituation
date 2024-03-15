#include "Animations/MeleeAnimNotify.h"
#include "Characters/PlayerCharacterBase.h"

void UMeleeAnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	PlayerCharacter = Cast<APlayerCharacterBase>(MeshComp->GetOwner());
	
	if(PlayerCharacter)
	{
		PlayerCharacter->ActivateMelee();
	}
}

void UMeleeAnimNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(PlayerCharacter)
	{
		PlayerCharacter->DeactivateMelee();
	}
}

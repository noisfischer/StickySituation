#include "Animations/CollectAmmoAnimNotify.h"
#include "Characters/PlayerCharacterBase.h"

void UCollectAmmoAnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	PlayerCharacter = Cast<APlayerCharacterBase>(MeshComp->GetOwner());
	
	if(PlayerCharacter)
		PlayerCharacter->CollectAmmo();
}
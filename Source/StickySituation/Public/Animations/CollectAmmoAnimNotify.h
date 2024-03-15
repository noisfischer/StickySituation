#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CollectAmmoAnimNotify.generated.h"

class APlayerCharacterBase;

UCLASS()
class STICKYSITUATION_API UCollectAmmoAnimNotify : public UAnimNotifyState
{
	GENERATED_BODY()

	UPROPERTY()
	APlayerCharacterBase* PlayerCharacter = nullptr;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
};

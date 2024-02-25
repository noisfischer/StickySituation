// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MeleeAnimNotify.generated.h"

class APlayerCharacterBase;

UCLASS()
class STICKYSITUATION_API UMeleeAnimNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY()
	APlayerCharacterBase* PlayerCharacter = nullptr;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};

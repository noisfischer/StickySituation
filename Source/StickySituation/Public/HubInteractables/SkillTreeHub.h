#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/PlayerCharacterBase.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "SkillTreeHub.generated.h"

UCLASS()
class STICKYSITUATION_API ASkillTreeHub : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkillTreeHub();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	USphereComponent* SphereCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	UWidgetComponent* SkillTreeMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuView")
	FTransform CameraTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuView")
	FTransform OriginalCameraTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuView")
	FTransform WidgetTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuView")
	TSubclassOf<UUserWidget> JeremySkillTree;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player")
	ACharacter* PlayerRef = nullptr;

protected:
	virtual void BeginPlay() override;

	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int BodyIndex,
		bool bSweep,
		const FHitResult& SweepResult
		);

	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int BodyIndex
		);


//	virtual void Tick(float DeltaTime) override;

};

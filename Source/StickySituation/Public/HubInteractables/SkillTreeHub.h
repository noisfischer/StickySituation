#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/PlayerCharacterBase.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "SkillTreeHub.generated.h"

UCLASS()
class STICKYSITUATION_API ASkillTreeHub : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkillTreeHub();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	USphereComponent* SphereCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	UWidgetComponent* SkillTreeMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuView")
	FTransform NewCameraTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuView")
	FTransform OriginalCameraTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuView")
	FTransform WidgetTransform;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuView")
	TSubclassOf<UUserWidget> JeremySkillTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuView")
    TSubclassOf<UUserWidget> AmySkillTree;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuView")
    TSubclassOf<UUserWidget> ClaySkillTree;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player")
	APlayerCharacterBase* PlayerRef = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Timeline")
	FTimeline CameraTransitionTimeline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
	UCurveFloat* TimelineCurve;
	
	UFUNCTION()
	void TimelineProgress(float Alpha);
	UFUNCTION()
	void TimelineFinished();
	
	bool bCameraTransitionActive = false;
	bool bHubInUse = false;
	bool bHubAvailable = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int BodyIndex,
		bool bSweep,
		const FHitResult& SweepResult
		);
	
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int BodyIndex
		);

	UFUNCTION(BlueprintCallable)
	void InputToInteract();

	UFUNCTION()
	void BindInteractionInput();

	UFUNCTION(BlueprintCallable)
	void RemoveWidgetClass();

	void SetSkillMenu();

	virtual void Tick(float DeltaTime) override;

};

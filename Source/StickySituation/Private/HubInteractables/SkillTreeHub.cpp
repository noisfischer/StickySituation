#include "HubInteractables/SkillTreeHub.h"

#include "EnhancedInputComponent.h"
#include "Characters/PlayerCharacterBase.h"
#include "Characters/Jeremy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ASkillTreeHub::ASkillTreeHub()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SkillTreeMenu = CreateDefaultSubobject<UWidgetComponent>(TEXT("SkillTreeMenu"));

	RootComponent = SceneComponent;
	SphereCollision->SetupAttachment(RootComponent);
	Camera->SetupAttachment(RootComponent);
	SkillTreeMenu->SetupAttachment(RootComponent);
	
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ASkillTreeHub::OnBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ASkillTreeHub::OnEndOverlap);
}

void ASkillTreeHub::BeginPlay()
{
	Super::BeginPlay();

	BindInteractionInput();
	
	if(TimelineCurve)
	{
		FOnTimelineFloat Progress;
		Progress.BindUFunction(this, FName("TimelineProgress"));
		
		FOnTimelineEvent OnTimelineFinished;
		OnTimelineFinished.BindUFunction(this, FName("TimelineFinished"));

		CameraTransitionTimeline.AddInterpFloat(TimelineCurve, Progress, FName("TimelineProgress"));
		CameraTransitionTimeline.SetTimelineFinishedFunc(OnTimelineFinished);
		CameraTransitionTimeline.SetLooping(false);
	}

	PlayerRef = Cast<APlayerCharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	ensure(PlayerRef);
	
	if(PlayerRef->IsA(AJeremy::StaticClass()) && JeremySkillTree)
		SkillTreeMenu->SetWidgetClass(JeremySkillTree);
	else
		UE_LOG(LogTemp, Warning, TEXT("Set value for JeremySkillTree"));

	/* FOR THE OTHER CHARACTERS, WHEN IMPLEMENTED
	if(PlayerRef->IsA(AAmy::StaticClass()) && AmySkillTree)
    		SkillTreeMenu->SetWidgetClass(AmySkillTree);
    	else
    		UE_LOG(LogTemp, Warning, TEXT("Set value for AmySkillTree"));
    if(PlayerRef->IsA(AClay::StaticClass()) && ClaySkillTree)
        		SkillTreeMenu->SetWidgetClass(ClaySkillTree);
        	else
        		UE_LOG(LogTemp, Warning, TEXT("Set value for ClaySkillTree"));
    */

	NewCameraTransform = Camera->GetComponentTransform();
}

void ASkillTreeHub::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bCameraTransitionActive)
		CameraTransitionTimeline.TickTimeline(DeltaTime);
}


void ASkillTreeHub::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int BodyIndex, bool bSweep, const FHitResult& SweepResult)
{
	if(OtherActor == PlayerRef)
		bHubAvailable = true;
}

void ASkillTreeHub::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int BodyIndex)
{
	if(OtherActor == PlayerRef)
		bHubAvailable = false;
}

void ASkillTreeHub::InputToInteract()
{
	if(PlayerRef->GetCharacterMovement()->IsFalling())
		return;
	
	if(!bHubInUse && bHubAvailable)
	{
		PlayerRef->GetCharacterMovement()->StopMovementImmediately();	// Prevents cam misplacement
		PlayerRef->GetController()->SetIgnoreMoveInput(true);
		PlayerRef->GetController()->SetIgnoreLookInput(true);
		
		FInputModeUIOnly CurrentInputMode;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(CurrentInputMode);

		OriginalCameraTransform = PlayerRef->FollowCamera->GetComponentTransform();
		
		CameraTransitionTimeline.Play();
		bCameraTransitionActive = true;
	}
	else if(bHubInUse && bHubAvailable)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
		CameraTransitionTimeline.Reverse();
	}
}

void ASkillTreeHub::TimelineProgress(float Alpha)
{
	FTransform CameraTransition = UKismetMathLibrary::TLerp(OriginalCameraTransform, NewCameraTransform, Alpha);
	PlayerRef->FollowCamera->SetWorldTransform(CameraTransition);
}

void ASkillTreeHub::TimelineFinished()
{
	if(!bHubInUse && bHubAvailable)
	{
		bHubInUse = true;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
	}
	else if(bHubInUse && bHubAvailable)
	{
		FInputModeGameOnly CurrentInputMode;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(CurrentInputMode);
		
		PlayerRef->GetController()->SetIgnoreMoveInput(false);
		PlayerRef->GetController()->SetIgnoreLookInput(false);

		bHubInUse = false;
		bCameraTransitionActive = false;
	}
}

void ASkillTreeHub::BindInteractionInput()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		UEnhancedInputComponent* PlayerInput = Cast<UEnhancedInputComponent>(PC->InputComponent);
		if (PlayerInput)
		{
			// Assuming InteractAction is a UInputAction* that's properly set up in your class
			PlayerInput->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ASkillTreeHub::InputToInteract);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Provide a mapping for IA_Interact"));
		}
	}
}

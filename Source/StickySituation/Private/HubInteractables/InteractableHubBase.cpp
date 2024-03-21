#include "HubInteractables/InteractableHubBase.h"
#include "EnhancedInputComponent.h"
#include "Characters/PlayerCharacterBase.h"
#include "Characters/Jeremy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AInteractableHubBase::AInteractableHubBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	HubMenu = CreateDefaultSubobject<UWidgetComponent>(TEXT("SkillTreeMenu"));

	RootComponent = SceneComponent;
	SphereCollision->SetupAttachment(RootComponent);
	Camera->SetupAttachment(RootComponent);
	HubMenu->SetupAttachment(RootComponent);
	
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AInteractableHubBase::OnBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AInteractableHubBase::OnEndOverlap);
}

void AInteractableHubBase::BeginPlay()
{
	Super::BeginPlay();
	
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
	BindInteractionInput();

	NewCameraTransform = Camera->GetComponentTransform();
}

void AInteractableHubBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bCameraTransitionActive)
		CameraTransitionTimeline.TickTimeline(DeltaTime);
}


void AInteractableHubBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int BodyIndex, bool bSweep, const FHitResult& SweepResult)
{
	if(OtherActor == PlayerRef)
	{
		bHubAvailable = true;
		PlayerRef->CurrentState = EPlayerState::CanInteract;
		UE_LOG(LogTemp, Warning, TEXT("CAN INTERACT"));

		if(HubMenu->GetWidgetClass() == nullptr)
			SetMenu();
	}
}

void AInteractableHubBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int BodyIndex)
{
	if(OtherActor == PlayerRef)
	{
		bHubAvailable = false;
		PlayerRef->CurrentState = EPlayerState::Neutral;
		UE_LOG(LogTemp, Warning, TEXT("NEUTRAL - end overlap"));
	}
}

void AInteractableHubBase::InputToInteract()
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

void AInteractableHubBase::TimelineProgress(float Alpha)
{
	FTransform CameraTransition = UKismetMathLibrary::TLerp(OriginalCameraTransform, NewCameraTransform, Alpha);
	PlayerRef->FollowCamera->SetWorldTransform(CameraTransition);
}

void AInteractableHubBase::TimelineFinished()
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

void AInteractableHubBase::BindInteractionInput()
{
	if (const APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (UEnhancedInputComponent* PlayerInput = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			// Assuming InteractAction is a UInputAction* that's properly set up in your class
			PlayerInput->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AInteractableHubBase::InputToInteract);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Provide a mapping for IA_Interact"));
	}
}

void AInteractableHubBase::RemoveWidgetClass()
{
	// Called in individual widget BPs so if character changes, correct widget will be constructed
	HubMenu->SetWidgetClass(nullptr);
}

void AInteractableHubBase::SetMenu()
{
	if(PlayerRef->IsA(AJeremy::StaticClass()) && JeremyMenu)
		HubMenu->SetWidgetClass(JeremyMenu);
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
}

#include "HubInteractables/SkillTreeHub.h"
#include "Characters/Jeremy.h"
#include "Kismet/GameplayStatics.h"

ASkillTreeHub::ASkillTreeHub()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SkillTreeMenu = CreateDefaultSubobject<UWidgetComponent>(TEXT("SkillTreeMenu"));

	RootComponent = Mesh;
	SphereCollision->SetupAttachment(Mesh);
	Camera->SetupAttachment(Mesh);
	SkillTreeMenu->SetupAttachment(Mesh);
	
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ASkillTreeHub::OnBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ASkillTreeHub::OnEndOverlap);
}

void ASkillTreeHub::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = UGameplayStatics::GetPlayerCharacter(this, 0);

	if(PlayerRef->IsA(AJeremy::StaticClass()) && JeremySkillTree)
		SkillTreeMenu->SetWidgetClass(JeremySkillTree);
	else
		UE_LOG(LogTemp, Warning, TEXT("Set value for JeremySkillTree"));

	// for if's for other 2 characters here
}

void ASkillTreeHub::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int BodyIndex, bool bSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Input Enabled"));
	
	if(OtherActor == PlayerRef)
	{
		EnableInput(GetWorld()->GetFirstPlayerController());
		UE_LOG(LogTemp, Warning, TEXT("Input Enabled"));
	}
}

void ASkillTreeHub::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int BodyIndex)
{
	if(OtherActor == PlayerRef)
	{
		EnableInput(GetWorld()->GetFirstPlayerController());
		UE_LOG(LogTemp, Warning, TEXT("Input Disabled"));
	}
}


/*
void ASkillTreeHub::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
*/


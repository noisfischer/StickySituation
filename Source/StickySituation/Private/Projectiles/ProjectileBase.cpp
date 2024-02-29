#include "Projectiles/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

// CONSTRUCTOR //
AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// SETUP PROJECTILE MESH //
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;


	// SETUP ARROW COMP //
	ForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ForwardArrow->SetupAttachment(ProjectileMesh);
	ForwardArrow->ArrowSize = 2.f;
	
	

	// SETUP PROJECTILE COMPONENT W/NO SPEED NOR GRAVITY
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->MaxSpeed = 5000.f;
	ProjectileMovementComponent->InitialSpeed = ProjectileMovementComponent->MaxSpeed;
	ProjectileMovementComponent->ProjectileGravityScale = 1.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->bIsHomingProjectile = false;
}



void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
}


void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AProjectileBase::PerformRadialBlast(float Radius)
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	
	GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(Radius),
		QueryParams
		);

	DrawDebugSphere(
		GetWorld(),
		GetActorLocation(),
		Radius,
		50,
		FColor::Red,
		false,
		2.f,
		0
		);

	if(HitResults.Num() > 0)
	{
		for(auto Hit : HitResults)
			if(Hit.GetActor()->ActorHasTag("enemy"))
				RadialBlastAction(Hit.GetActor());
	}
}

void AProjectileBase::RadialBlastAction(AActor* Enemy)
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Hit"));
}


void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* HitActor, UPrimitiveComponent* OtherComp,
                            FVector NormalImpulse, const FHitResult& Hit)
{
	if(HitActor && HitActor->ActorHasTag("enemy"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Direct Hit!"));

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ProjectileImpactSystem,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
			);

		UGameplayStatics::PlaySoundAtLocation(
			this,
			DirectImpactSound,
			Hit.Location
			);
	}

	else
	{

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ProjectileImpactSystem,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
			);

		UGameplayStatics::PlaySoundAtLocation(
			this,
			DestroyedSound,
			Hit.Location
			);
	}

	UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), GetCurrentSpeed());
	UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), Damage);
}


float AProjectileBase::GetDamage()
{
	return Damage;
}

void AProjectileBase::SetDamage(float NewDamage)
{
	Damage = NewDamage;
}

float AProjectileBase::GetCurrentSpeed()
{
	return ProjectileMovementComponent->MaxSpeed;
}

void AProjectileBase::SetCurrentSpeed(float NewSpeed)
{
	ProjectileMovementComponent->MaxSpeed = NewSpeed;
	ProjectileMovementComponent->InitialSpeed = ProjectileMovementComponent->MaxSpeed;
}

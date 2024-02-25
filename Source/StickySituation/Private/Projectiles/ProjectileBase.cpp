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

	// BIND "OnHit" FUNCTION TO "OnComponentHit" EVENT OF "ProjectileMesh" //
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	
}


void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* HitActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(HitActor && HitActor->ActorHasTag("enemy"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Hit!"));

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ProjectileImpactSystem,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
			);

		UGameplayStatics::PlaySoundAtLocation(
			this,
			ProjectileImpactSound,
			Hit.Location
			);

		this->Destroy();
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
			ProjectileImpactSound,
			Hit.Location
			);

		this->Destroy();
	}
}

float AProjectileBase::GetBaseDamage()
{
	return BaseDamage;
}

void AProjectileBase::SetBaseDamage(float ChargeValue)
{
	BaseDamage = BaseDamage * ChargeValue;
}

float AProjectileBase::GetCurrentSpeed()
{
	return ProjectileMovementComponent->MaxSpeed;
}

void AProjectileBase::SetCurrentSpeed(float ChargeValue)
{
	ProjectileMovementComponent->MaxSpeed = GetCurrentSpeed() * ChargeValue;
	ProjectileMovementComponent->InitialSpeed = ProjectileMovementComponent->MaxSpeed;
}

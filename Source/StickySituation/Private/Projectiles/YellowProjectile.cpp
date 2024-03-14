#include "Projectiles/YellowProjectile.h"

AYellowProjectile::AYellowProjectile()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AYellowProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->SetWorldScale3D(SphereCollision->GetComponentScale() * AcidScale);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AYellowProjectile::AcidBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AYellowProjectile::AcidEndOverlap);
}

void AYellowProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* HitActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComponent, HitActor, OtherComp, NormalImpulse, Hit);

	ProjectileMesh->SetVisibility(false);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	GetWorld()->GetTimerManager().SetTimer(
		AcidDurationTimer,
		this,
		&AYellowProjectile::EndAcidTimer,
		AcidDuration,
		false
		);
	GetWorld()->GetTimerManager().SetTimer(
		AcidDamageTimer,
		this,
		&AYellowProjectile::ApplyAcidDamage,
		AcidDamageRate,
		true
		);
}

void AYellowProjectile::EndAcidTimer()
{
	AcidDurationTimer.Invalidate();
	AcidDamageTimer.Invalidate();
	AffectedEnemies.Empty();
	this->Destroy();
}

void AYellowProjectile::ApplyAcidDamage()
{
	// REMOVE ENEMIES FROM ARRAY IF DEAD //
	for(int32 i = AffectedEnemies.Num() - 1; i >= 0; --i)
	{
		if (!AffectedEnemies[i])
			AffectedEnemies.RemoveAt(i);
	}
	
	for(auto enemy : AffectedEnemies)
		Execute_DamageEnemy(enemy, AcidDamageAmount);
}

void AYellowProjectile::AcidBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("enemy"))
	{
		AffectedEnemies.Add(OtherActor);
		UE_LOG(LogTemp, Warning, TEXT("Enemies Overlapped: %i"), AffectedEnemies.Num());
	}
}

void AYellowProjectile::AcidEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(AffectedEnemies.Contains(OtherActor))
	{
		AffectedEnemies.Remove(OtherActor);
		UE_LOG(LogTemp, Warning, TEXT("Enemies Overlapped: %i"), AffectedEnemies.Num());
	}
}
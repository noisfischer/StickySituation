#include "Projectiles/RedProjectile.h"

ARedProjectile::ARedProjectile()
{
	//
}

void ARedProjectile::BeginPlay()
{
	Super::BeginPlay();
}


void ARedProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* HitActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComponent, HitActor, OtherComp, NormalImpulse, Hit);

	PerformRadialBlast(BlastRadius);

	this->Destroy();
}

void ARedProjectile::RadialBlastAction(AActor* Enemy)
{
	Super::RadialBlastAction(Enemy);

	Execute_DamageEnemy(Enemy, BaseDamage);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

// Sets default values
ABullet::ABullet() :
	CollisionSphereScale(0.1f, 0.1f, 0.1f)
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit);		// set up a notification for when this component hits something blocking
	
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	if (BulletMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bullet mesh is valid"));
		BulletMeshComponent->SetStaticMesh(BulletMesh);
		BulletMeshComponent->SetRelativeScale3D(CollisionSphereScale);
	}
	BulletMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMeshComponent->SetupAttachment(RootComponent);
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2500.f;
	ProjectileMovement->MaxSpeed = 2500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 10.0f;
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		TArray<FName> RowNames = HitInteractionRules->GetRowNames();
		for (FName RowIndex : RowNames)
		{
			const FString ContextText;
			FHitInteractionRules* Row = HitInteractionRules->FindRow<FHitInteractionRules>(RowIndex, ContextText, true);
			if (Row && Row->CollidedComponent->GetName().Contains(OtherActor->GetClass()->GetName()))
			{
				FTransform Transform;
				OtherActor->AddComponentByClass(Row->ComponentToInteract, true, Transform, false);
			}
		}
	}
	Destroy();
}

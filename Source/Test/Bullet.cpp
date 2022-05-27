// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABullet::ABullet()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Bullet");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit);		// set up a notification for when this component hits something blocking
	
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;
	
	BulletMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMeshComponent->SetupAttachment(RootComponent);
	
	// Use a ProjectileMovementComponent to govern this projectile's movement
	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletComp"));
	BulletMovement->UpdatedComponent = CollisionComp;
	BulletMovement->InitialSpeed = 3500.f;
	BulletMovement->MaxSpeed = 3500.f;
	BulletMovement->bRotationFollowsVelocity = true;
	BulletMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (bAffectMultipleActors)
	{
		SetAoEDamage();
	}
	else
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
					OnBulletHitDelegate.ExecuteIfBound(OtherActor, Row->ComponentToInteract);
				}
			}
		}
	}
	if (bDestroyOnHit)
	{
		Destroy();
	}
}

void ABullet::SetAoEDamage()
{
	// Set what actors to seek out from it's collision channel
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	
	// Ignore any specific actors
	TArray<AActor*> IgnoreActors;
	// Ignore self or remove this line to not ignore any
	IgnoreActors.Add(this);
	
	// Array of actors that are inside the radius of the sphere
	TArray<AActor*> CollidedActorsArray;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), this->GetActorLocation(), AoESphereRadius, TraceObjectTypes, nullptr, IgnoreActors, CollidedActorsArray);

	DrawDebugSphere(GetWorld(), GetActorLocation(), AoESphereRadius, 12, FColor::Red, false, 2.0f);

	TArray<FName> RowNames = HitInteractionRules->GetRowNames();
	for (AActor* CollidedActor : CollidedActorsArray)
	{
		for (FName RowIndex : RowNames)
		{
			const FString ContextText;
			FHitInteractionRules* Row = HitInteractionRules->FindRow<FHitInteractionRules>(RowIndex, ContextText, true);
			if (Row && Row->CollidedComponent->GetName().Contains(CollidedActor->GetClass()->GetName()))
			{
				FTransform Transform;
				CollidedActor->AddComponentByClass(Row->ComponentToInteract, true, Transform, false);
			}
		}
	}
}
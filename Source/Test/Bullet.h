// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DataTable.h"
#include "Bullet.generated.h"


USTRUCT(BlueprintType)
struct FHitInteractionRules : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UClass* CollidedComponent;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UActorComponent> ComponentToInteract;
};

UCLASS(config=Game)
class TEST_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

private:
	/** Bullet Mesh */
	UPROPERTY(EditAnywhere, Category = "Bullet")
	UStaticMesh* BulletMesh;

	UPROPERTY(EditAnywhere, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BulletMeshComponent;
	
	/** Sphere collision component */
	UPROPERTY(EditAnywhere, Category = "Bullet")
	USphereComponent* CollisionComp;

	/** Scale of collision Box */
	UPROPERTY(EditAnywhere, Category = "Bullet")
	FVector CollisionSphereScale;
	
	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	UDataTable* HitInteractionRules;
public:
	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

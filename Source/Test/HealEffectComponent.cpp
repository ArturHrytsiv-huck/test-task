// Fill out your copyright notice in the Description page of Project Settings.


#include "HealEffectComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values for this component's properties
UHealEffectComponent::UHealEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	const auto HealEffectObject = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/Game/VFX/P_Revenant_Primary_MuzzleFlash.P_Revenant_Primary_MuzzleFlash'"));
	
	if (HealEffectObject.Object)
	{
		// HealEffect = Cast<UParticleSystem>(HealEffect);
		AActor* Parent = GetOwner();
		if (Parent)
		{
			UParticleSystem* HealEffect = HealEffectObject.Object;
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HealEffect, Parent->GetActorLocation());
		}
	}
}


// Called when the game starts
void UHealEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


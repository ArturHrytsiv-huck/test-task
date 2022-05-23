// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyComponent.h"

// Sets default values for this component's properties
UDestroyComponent::UDestroyComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	AActor* Parent = GetOwner();
	if (Parent)
	{
		Parent->Destroy();
	}

}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CharacterHUD.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API ACharacterHUD : public AHUD
{
	GENERATED_BODY()
public:
	ACharacterHUD();
	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTexture;

};

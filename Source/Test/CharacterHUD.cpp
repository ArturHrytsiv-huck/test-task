// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterHUD.h"

#include "CanvasItem.h"
#include "Engine/Canvas.h"

ACharacterHUD::ACharacterHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/Game/HUD/FirstPersonCrosshair'"));
	CrosshairTexture = CrosshairTexObj.Object;
}

void ACharacterHUD::DrawHUD()
{
	Super::DrawHUD();
	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X - 50.f),
										   (Center.Y));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}

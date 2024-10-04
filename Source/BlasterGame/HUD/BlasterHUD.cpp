// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterHUD.h"
#include "GameFramework/PlayerController.h"
#include "CharacterOverlay.h"

void ABlasterHUD::BeginPlay()
{
	Super::BeginPlay();

	AddCharacterOverlay();
}

void ABlasterHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && CharacterOverlayClass) {
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}

// This is where we get out HUD and then use this information so we can pass in out Textures and Draw the actual CrossHair.
void ABlasterHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;
	if (GEngine) {

		GEngine->GameViewport->GetViewportSize(ViewportSize);	// Assigning our ViewportSize variable.

		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);	// This is how we get the coordinates of the center of our Viewport.

		float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;	// This is how we will Spread our Crosshair in the Direction of either X or Y.

		if (HUDPackage.CrosshairsCenter) {
			FVector2D Spread(0.f, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter, Spread, HUDPackage.CrosshairsColor);	// We'll pass in the texture that we assigned and at the location.
		}
		if (HUDPackage.CrosshairsLeft) {
			FVector2D Spread(-SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsRight) {
			FVector2D Spread(SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsTop) {
			FVector2D Spread(0.f, -SpreadScaled);	// Weird thing about UV coordinates is that moving upward is in the Negative Direction.
			DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsBottom) {
			FVector2D Spread(0.f, SpreadScaled);	// Weird thing about UV coordinates is that moving downward is in the Positive Direction.
			DrawCrosshair(HUDPackage.CrosshairsBottom, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
	}
}

// This is where we draw our Crosshair at the center of the screen.
void ABlasterHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor)
{
	// Need some math so texture can be at the center.
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(	// Draw point needs to be at the center of the screen, moved to the left by texture, width divided by 2 and height divided by 2.
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,	// We only need to move it over by half of its width.
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y
	);

	DrawTexture(
		Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		CrosshairColor	// White is it's original color.
	);
}

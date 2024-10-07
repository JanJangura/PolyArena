// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

// Struct that helps us define our Textures and setting it equal.
USTRUCT(BlueprintType)
struct FHUDPackage {
	GENERATED_BODY()
public:
	class UTexture2D* CrosshairsCenter;
	UTexture2D* CrosshairsLeft;
	UTexture2D* CrosshairsRight;
	UTexture2D* CrosshairsTop;
	UTexture2D* CrosshairsBottom;
	float CrosshairSpread;
	FLinearColor CrosshairsColor;
};


/**
 * 
 */
UCLASS()
class BLASTERGAME_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;

	// This is so we can select in the Blaster HUD Blueprint and allow us to have the class we need when we'd like to create the Widget.
	// Once we set this in our Blueprint, we can then create this Widget.
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;	// This is the CharacterOverlay Class that we'll need tp specify from BP.

	class UCharacterOverlay* CharacterOverlay;

protected:
	virtual void BeginPlay() override;
	void AddCharacterOverlay();	// This is a function to Add our Character Overlay Widget to this BlasterHUD.

private:
	FHUDPackage HUDPackage;

	// This is where we draw our crosshair
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;

public:
	class ABlasterCharacter* Character;

	bool PauseUICreated = false;

	APlayerController* PlayerController;
	class ABlasterPlayerController* BlasterPlayerController;

	void DeclarationOfClasses();

	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
	FORCEINLINE bool RetrievePauseUICreated() { return PauseUICreated; }
	FORCEINLINE APlayerController* RetrievePlayerController() { return PlayerController; }
};

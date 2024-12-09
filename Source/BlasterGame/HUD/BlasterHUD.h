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

	void AddCharacterOverlay();	// This is a function to Add our Character Overlay Widget to this BlasterHUD.
	
	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;
	
	UPROPERTY(EditAnywhere, Category = "Announcements")
	TSubclassOf<UUserWidget> AnnouncementClass;

	UPROPERTY()
	class UAnnouncement* Announcement;

	void AddAnouncement();
	void AddElimAnnouncement(FString Attacker, FString Victim);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class APlayerController* OwningPlayer;

	FHUDPackage HUDPackage;

	// This is where we draw our crosshair
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;

	UPROPERTY(EditAnywhere, Category = "Announcements")
	TSubclassOf<class UElimAnnouncement> ElimAnnouncementClass;

	UPROPERTY(EditAnywhere, Category = "Announcements")
	float ElimAnnouncementTime = 3.f;

	UFUNCTION()
	void ElimAnnouncementTimerFinished(UElimAnnouncement* MsgToRemove);

	UPROPERTY()
	TArray<UElimAnnouncement*> ElimMessages;

	// PlayerList 
	UPROPERTY(EditAnywhere, Category = "Announcements")
	TSubclassOf<class UUserWidget> PlayerListWidget;

	class UPlayerList* PlayerList;

	class ABlasterGameState* BlasterGameState;

	void InitiatePlayerListWidget(class ABlasterPlayerController* BlasterPlayerController);
	void PopulatePlayerListWidget();

	FTimerHandle DelayPlayerList;

public:

	bool PauseUICreated = false;

	APlayerController* PlayerController;

	void DeclarationOfClasses();

	void UpdatePlayerList(TArray<class ABlasterPlayerState*> NewBlasterPlayerState);

	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
	FORCEINLINE bool RetrievePauseUICreated() { return PauseUICreated; }
	FORCEINLINE APlayerController* RetrievePlayerController() { return PlayerController; }
};

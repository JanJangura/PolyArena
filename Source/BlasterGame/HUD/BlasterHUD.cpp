// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterHUD.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "CharacterOverlay.h"
#include "BlasterGame/Character/BlasterCharacter.h"
#include "Kismet/GameplayStatics.h" 
#include "LaunchGameButton.h"
#include "BlasterGame/PlayerController/BlasterPlayerController.h"
#include "Announcement.h"
#include "ElimAnnouncement.h"
#include "Components/HorizontalBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "PlayerList.h"
#include "Components/ScrollBox.h"
#include "Components/Widget.h"
#include "BlasterGame/GameState/BlasterGameState.h"
#include "BlasterGame/PlayerState/BlasterPlayerState.h"

void ABlasterHUD::BeginPlay()
{
	Super::BeginPlay();
	BlasterGameState = GetWorld()->GetGameState<ABlasterGameState>();

	if (BlasterGameState && IsValid(BlasterGameState)) {
		//BlasterGameState->OnPlayerListUpdate.AddDynamic(this, &ABlasterHUD::BroadCastPlayerToPlayerList);
	}

	DeclarationOfClasses();

	if(HasAuthority()) GetWorld()->GetTimerManager().SetTimer(DelayPlayerList, this, &ABlasterHUD::PopulatePlayerListWidget, 3.0f, false);
}

void ABlasterHUD::PopulatePlayerListWidget()
{
	if (UWorld* World = GetWorld())
	{
		// Attempt to get the GameState
		BlasterGameState = BlasterGameState == nullptr ? World->GetGameState<ABlasterGameState>() : BlasterGameState;

		if (BlasterGameState)
		{
			//BlasterGameState->UpdatePlayerList();
		}
	}
}

// This is where we create our Widget
void ABlasterHUD::AddCharacterOverlay()
{
	PlayerController = GetOwningPlayerController();

	if (PlayerController && CharacterOverlayClass) {	// If our Player Controller and CharacterOverlayClass Widget is valid.

		// We'll Create the Widget and then define which Class we're using, which is UCharacterOverlay, then what PlayerController, and then what Widget. Then
		// CharacterOverlay is the object of this Class that we have specified. 
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass); 

		// Now we'll add this to our Viewport.
		CharacterOverlay->AddToViewport();	
	}
}

void ABlasterHUD::AddAnouncement()
{
	PlayerController = GetOwningPlayerController();

	if (PlayerController && AnnouncementClass) {	// If our Player Controller and Announcement Widget is valid.

		Announcement = CreateWidget<UAnnouncement>(PlayerController, AnnouncementClass);

		// Now we'll add this to our Viewport.
		Announcement->AddToViewport();
	}
}

void ABlasterHUD::AddElimAnnouncement(FString Attacker, FString Victim)
{
	OwningPlayer = OwningPlayer == nullptr ? GetOwningPlayerController() : OwningPlayer;
	if (OwningPlayer && ElimAnnouncementClass) {

		UElimAnnouncement* ElimAnnouncementWidget = CreateWidget<UElimAnnouncement>(OwningPlayer, ElimAnnouncementClass);
		if (ElimAnnouncementWidget) {
			ElimAnnouncementWidget->SetElimAnnouncementText(Attacker, Victim);
			ElimAnnouncementWidget->AddToViewport();
			ElimMessages.Add(ElimAnnouncementWidget);

			for (UElimAnnouncement* Msg : ElimMessages) {
				if (Msg && Msg->AnnouncementBox) {
					UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Msg->AnnouncementBox);
					FVector2D Position = CanvasSlot->GetPosition();
					FVector2D NewPosition(CanvasSlot->GetPosition().X,
						Position.Y - CanvasSlot->GetSize().Y
					);
					CanvasSlot->SetPosition(NewPosition);
				}
			}

			// Setting a Timer passed in a Delegate. The Delegate has the callback to ElimAnnouncementTimerFinished bound to it and we're passing in the Widget itself. Then once
			// the Timer is finished, we'll remove that Widget. This only handles Removing the Widget, it won't push the Widget up.
			FTimerHandle ElimMsgTimer;
			FTimerDelegate ElimMsgDelegate;
			ElimMsgDelegate.BindUFunction(this, FName("ElimAnnouncementTimerFinished"), ElimAnnouncementWidget);

			GetWorldTimerManager().SetTimer(
				ElimMsgTimer,
				ElimMsgDelegate,
				ElimAnnouncementTime,
				false
			);
		}
	}
}

void ABlasterHUD::ElimAnnouncementTimerFinished(UElimAnnouncement* MsgToRemove)
{
	if (MsgToRemove) {
		MsgToRemove->RemoveFromParent();
	}
}

void ABlasterHUD::DeclarationOfClasses()
{
	// We need the Player Controller in order to create our Widget, because that's where we have to access in order to use our Widget.
	PlayerController = GetOwningPlayerController();
	ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(PlayerController);

	if (PlayerController) {
		APawn* ControlledPawn = PlayerController->GetPawn();

		if (ControlledPawn) {
			class ABlasterCharacter* Character = Cast<ABlasterCharacter>(ControlledPawn);
		}
	}

	InitiatePlayerListWidget(BlasterPlayerController);
}

void ABlasterHUD::InitiatePlayerListWidget(ABlasterPlayerController* BlasterPlayerController)
{
	if (PlayerListWidget && BlasterPlayerController) {
		PlayerList = CreateWidget<UPlayerList>(GetWorld(), PlayerListWidget);

		if (PlayerList) {
			BlasterPlayerController->PlayerList = PlayerList;
		}
	}
}

void ABlasterHUD::UpdatePlayerList(const TArray<APlayerState*>& PlayerArray)
{
	if (!PlayerList) return;

	UE_LOG(LogTemp, Warning, TEXT("PlayerList is valid!"));

	if (PlayerList && PlayerList->PlayerScrollBox) {
		PlayerList->PlayerScrollBox->ClearChildren();

		for (APlayerState* PS : PlayerArray) {
			ABlasterPlayerState* BPS = Cast<ABlasterPlayerState>(PS);
			if (BPS) {
				FString PlayerName = BPS->GetPlayerName();
				int32 InitialKillScore = BPS->GetScore();
				int32 InitialDeathScore = BPS->Defeats;

				PlayerList->AddPlayerInfoWidget(PlayerName, InitialKillScore, InitialDeathScore);
			}
		}
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

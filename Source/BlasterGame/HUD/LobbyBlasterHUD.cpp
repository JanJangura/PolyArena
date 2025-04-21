// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyBlasterHUD.h"
#include "Loading.h"
#include "BlasterGame/HUD/LaunchGameButton.h"
#include "BlasterGame/GameState/LobbyBlasterGameState.h"
#include "LobbyPlayerList.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "BlasterGame/GameMode/LobbyGameMode.h"
#include "Components/TextBlock.h"
#include "BlasterGame/GameMode/LobbyGameMode.h"

ALobbyBlasterHUD::ALobbyBlasterHUD() {
	static ConstructorHelpers::FClassFinder<ULaunchGameButton> WidgetClassOne(TEXT("/Game/BP_Shooter_Character/Blueprints/HUD/WBP_LaunchGameButton"));
	static ConstructorHelpers::FClassFinder<ULoading> WidgetClassTwo(TEXT("/Game/BP_Shooter_Character/Blueprints/HUD/WBP_Loading"));
	if (WidgetClassOne.Succeeded())
	{
		LaunchGameButtonClass = WidgetClassOne.Class;
	}
	if (WidgetClassTwo.Succeeded())
	{
		LoadingClass = WidgetClassTwo.Class;
	}
}

void ALobbyBlasterHUD::BeginPlay()
{
	Super::BeginPlay();

	AddPauseUI();
	ShowPlayerList();

	LobbyBlasterGameState = GetWorld()->GetGameState<ALobbyBlasterGameState>();

	if (LobbyBlasterGameState)
	{
		// Defer broadcasting to ensure everything is set up
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			if (LobbyBlasterGameState)
			{
				LobbyBlasterGameState->GetPlayersFromGameInstance();
			}
		});
	}
}

void ALobbyBlasterHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*if (PlayerController) {
		if (PlayerController->HasAuthority()) {
			UE_LOG(LogTemp, Warning, TEXT("Server Loading is %s"), LoadingText != nullptr ? TEXT("Valid") : TEXT("Invalid"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Client Loading is %s"), LoadingText != nullptr ? TEXT("Valid") : TEXT("Invalid"));
		}
	}*/
}

void ALobbyBlasterHUD::ShowPlayerList()
{
	/*
	APlayerController* PC = GetOwningPlayerController();
	if (PC && LobbyPlayerListClass) {
		LobbyPlayerList = CreateWidget<ULobbyPlayerList>(PC, LobbyPlayerListClass);
		if (LobbyPlayerList) {
			LobbyPlayerList->AddToViewport();
		}
	}
	*/
}

void ALobbyBlasterHUD::UpdatePlayerList(FString PlayerName)
{
	if (LobbyPlayerList) {
		LobbyPlayerList->AddPlayerInfoWidget(PlayerName);
	}
}

void ALobbyBlasterHUD::AddPauseUI()
{
	if (PlayerController && LoadingClass) {

		/*
		if (LaunchGameButtonClass) {
			LaunchGameButton = CreateWidget<ULaunchGameButton>(PlayerController, LaunchGameButtonClass);
			LaunchGameButton->AddToViewport();
			LaunchGameButton->SetVisibility(ESlateVisibility::Hidden);
		}

		if (LoadingClass) {
			LoadingText = CreateWidget<ULoading>(PlayerController, LoadingClass);
			LoadingText->AddToViewport();

			FString AnnouncementText("Press P to Launch Game");
			LoadingText->Loading->SetText(FText::FromString(AnnouncementText));
			LoadingText->SetVisibility(ESlateVisibility::Visible);
		}
		PauseUICreated = true; */

		LoadingText = CreateWidget<ULoading>(PlayerController, LoadingClass);
		LoadingText->AddToViewport();

		FString AnnouncementText;
		if (PlayerController->HasAuthority()) {
			AnnouncementText = "Press P to Launch Game";
		}
		else {
			AnnouncementText = "Waiting for Host to Start";
		}

		LoadingText->Loading->SetText(FText::FromString(AnnouncementText));
		LoadingText->SetVisibility(ESlateVisibility::Visible);		
	}
}


void ALobbyBlasterHUD::HideLaunchGameButton()
{
	if (LaunchGameButton) {
		LaunchGameButton->SetVisibility(ESlateVisibility::Hidden);
	}

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(false);
	PlayerController->bShowMouseCursor = false;
}

void ALobbyBlasterHUD::ShowLaunchGameButton()
{
	if (LaunchGameButton) {
		LaunchGameButton->SetVisibility(ESlateVisibility::Visible);
	}

	FInputModeGameAndUI InputModeData;
	InputModeData.SetWidgetToFocus(LaunchGameButton->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(true);
}

void ALobbyBlasterHUD::ShowLoadingText()
{
	if (LoadingText) {
		UE_LOG(LogTemp, Warning, TEXT("LOADING CALLED!"));
		FString AnnouncementText("Loading...");
		LoadingText->Loading->SetText(FText::FromString(AnnouncementText));
		//HideLaunchGameButton();
	}
}

void ALobbyBlasterHUD::LaunchGame()
{
	AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);

	if (GameModeBase) {
		ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GameModeBase);

		if (LobbyGameMode) {
			ShowLoadingText();
			LobbyGameMode->ServerTravelToGame();
		}
	}
}

void ALobbyBlasterHUD::ToggleLaunchGameButton()
{
	if (PauseUICreated && PlayerController) {

		if (LaunchGameButton && LaunchGameButton->IsVisible()) {
			HideLaunchGameButton();
		}
		else {
			ShowLaunchGameButton();
		}
	}
}


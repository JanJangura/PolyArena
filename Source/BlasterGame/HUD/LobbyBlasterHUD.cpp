// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyBlasterHUD.h"
#include "Loading.h"
#include "BlasterGame/HUD/LaunchGameButton.h"
#include "BlasterGame/GameState/LobbyBlasterGameState.h"
#include "LobbyPlayerList.h"

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
	if (PlayerController && LobbyPlayerListClass) {
		LobbyPlayerList = CreateWidget<ULobbyPlayerList>(PlayerController, LobbyPlayerListClass);

		if (LobbyPlayerList) LobbyPlayerList->AddToViewport();
	}
}

void ALobbyBlasterHUD::UpdatePlayerList(FString PlayerName)
{
	if (LobbyPlayerList) {
		LobbyPlayerList->AddPlayerInfoWidget(PlayerName);
	}
}

void ALobbyBlasterHUD::AddPauseUI()
{
	if (PlayerController) {

		if (LaunchGameButtonClass) {
			LaunchGameButton = CreateWidget<ULaunchGameButton>(PlayerController, LaunchGameButtonClass);
			LaunchGameButton->AddToViewport();
			LaunchGameButton->SetVisibility(ESlateVisibility::Hidden);
		}
		if (LoadingClass) {
			LoadingText = CreateWidget<ULoading>(PlayerController, LoadingClass);
			LoadingText->AddToViewport();
			LoadingText->SetVisibility(ESlateVisibility::Hidden);
		}
		PauseUICreated = true;
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
		LoadingText->SetVisibility(ESlateVisibility::Visible);
		HideLaunchGameButton();
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


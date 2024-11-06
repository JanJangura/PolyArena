// Fill out your copyright notice in the Description page of Project Settings.


#include "ReturnToMainMenu.h"
#include "GameFramework/PlayerController.h" // This allows us to access the controller
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "GameFramework/GameModeBase.h"

void UReturnToMainMenu::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	this->SetIsFocusable(true); // Boolean that focuses on the Widget.

	UWorld* World = GetWorld();
	if (World) {
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController) {
			FInputModeGameAndUI InputModeData; // Focus on the game and the UI.
			InputModeData.SetWidgetToFocus(TakeWidget()); // We'll set this Widget to Focus. 
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	// Binding Delegates
	if (ReturnButton && !ReturnButton->OnClicked.IsBound()) { // We only want to add dynamic here if this Delegate is not already binded.
		ReturnButton->OnClicked.AddDynamic(this, &UReturnToMainMenu::ReturnButtonClicked); // This is a Delegate (how events are activate).
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance) {

		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem) {
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &UReturnToMainMenu::OnDestroyedSession); 
		}
	}
}

bool UReturnToMainMenu::Initialize()
{
	if (!Super::Initialize()) { return false; } // If fails then return false.

	return true;
}

void UReturnToMainMenu::OnDestroyedSession(bool bWasSuccessful)
{
	if (!bWasSuccessful) {
		ReturnButton->SetIsEnabled(true);
		return;
	}

	UWorld* World = GetWorld();
	if (World) {

		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>(); // This will check if the Authoritive GameMode is casted upon the Server and not the Clients.
		if (GameMode) { 
			GameMode->ReturnToMainMenuHost(); // This will return the Host to MainMenu
		}
		else { // If the Gamemode is null, then that means we're on a Client.
			PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
			if (PlayerController) {
				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
			}
		}
	}
}

void UReturnToMainMenu::MenuTearDown()
{
	RemoveFromParent();

	// This will handle taking the Menu down.
	UWorld* World = GetWorld();
	if (World) {
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController) {
			FInputModeGameOnly InputModeData; // Focus on the game only
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}

	// Removing Delegates
	if (ReturnButton && ReturnButton->OnClicked.IsBound()) { // We'll remove dynamic here for this already Binded Delegate.
		ReturnButton->OnClicked.RemoveDynamic(this, &UReturnToMainMenu::ReturnButtonClicked); // This is a Delegate (how events are activate).
	}
	if (MultiplayerSessionsSubsystem && MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.IsBound()) {
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(this, &UReturnToMainMenu::OnDestroyedSession);
	}
}

void UReturnToMainMenu::ReturnButtonClicked()
{
	ReturnButton->SetIsEnabled(false); // Disable the ReturnButton

	if (MultiplayerSessionsSubsystem) {
		MultiplayerSessionsSubsystem->DestroySession();
	}
}
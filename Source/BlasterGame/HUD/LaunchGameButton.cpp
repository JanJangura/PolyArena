// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchGameButton.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "BlasterGame/GameMode/LobbyGameMode.h"
#include "BlasterGame/HUD/LobbyBlasterHUD.h"
#include "BlasterGame/PlayerController/BlasterPlayerController.h"


void ULaunchGameButton::NativeConstruct()
{
	if (LaunchGameButton) {
		LaunchGameButton->OnClicked.AddDynamic(this, &ULaunchGameButton::LaunchButtonClicked);

	}
	UE_LOG(LogTemp, Warning, TEXT("Called 4"));
	Super::NativeConstruct();
}

void ULaunchGameButton::LaunchButtonClicked()
{
	LaunchGameButton->SetIsEnabled(false);

	AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);

	if (GameModeBase) {
		ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GameModeBase);
		LobbyTearDown();
		UE_LOG(LogTemp, Warning, TEXT("Called 1"));
		
		
		if (LobbyGameMode) {
			LobbyGameMode->LaunchGameButtonClicked = true;
			LobbyGameMode->ServerJumpToGame(LobbyGameMode->MaxPlayers);
			UE_LOG(LogTemp, Warning, TEXT("LaunchGameButtonClicked"));
		}
		
		
	}
	
}

void ULaunchGameButton::LobbyTearDown()
{
	RemoveFromParent(); // To Remove Widget

	UWorld* World = GetWorld(); // Get Player Controller from the world
	if (World) {
		APlayerController* PlayerController = GetOwningPlayer(); //This is how we get our first player controller. 
		if (PlayerController) {
			ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(PlayerController);

			if (BlasterPlayerController) {
				UE_LOG(LogTemp, Warning, TEXT("Called 2"));
				BlasterPlayerController->SetNewHUD();
			}
			/*
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
			PlayerController->bShowMouseCursor = false;
			*/
			LobbyBlasterHUD = LobbyBlasterHUD == nullptr ? Cast<ALobbyBlasterHUD>(PlayerController->GetHUD()) : LobbyBlasterHUD;
			if (LobbyBlasterHUD) {
				LobbyBlasterHUD->HideLaunchGameButton();
				UE_LOG(LogTemp, Warning, TEXT("Called 3"));
			}
			

			
		}
	}
}

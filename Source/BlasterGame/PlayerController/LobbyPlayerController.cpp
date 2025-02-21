// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "BlasterGame/PlayerController/LobbyPlayerController.h"
#include "BlasterGame/HUD/LobbyBlasterHUD.h"
#include "GameFramework/PlayerState.h"
#include "BlasterGame/GameState/LobbyBlasterGameState.h"
#include "GameFramework/GameStateBase.h"


void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	LobbyBlasterHUD = Cast<ALobbyBlasterHUD>(GetHUD());
}

void ALobbyPlayerController::UpdatePlayerListInfo(TArray<FString> PlayerName)
{
	LobbyBlasterHUD = LobbyBlasterHUD == nullptr ? Cast<ALobbyBlasterHUD>(GetHUD()) : LobbyBlasterHUD;

	if (LobbyBlasterHUD) {
		UE_LOG(LogTemp, Warning, TEXT("lobbyblaster Called"));
		for (FString Names : PlayerName) {
			LobbyBlasterHUD->UpdatePlayerList(Names);
		}
	}
	else{
		UE_LOG(LogTemp, Warning, TEXT("lobbyblaster NOT Called"));
	}
}
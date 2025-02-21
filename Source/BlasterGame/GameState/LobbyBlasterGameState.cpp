// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyBlasterGameState.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "BlasterGame/PlayerController/LobbyPlayerController.h"
#include "BlasterGame/GameInstance/BlasterGameInstance.h"

void ALobbyBlasterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyBlasterGameState, MultiPlayerStates);
}

void ALobbyBlasterGameState::BeginPlay()
{
	Super::BeginPlay();
	GetPlayersFromGameInstance();
	BlasterGameInstance = Cast<UBlasterGameInstance>(GetGameInstance());
}

void ALobbyBlasterGameState::OnRep_MultiPlayerStates()
{
	if (MultiPlayerStates.Num() <= 0) return;

	for (APlayerState* Player : MultiPlayerStates) {
		if (Player) // Ensure PlayerState is valid
		{
			// Use GetOwner() to retrieve the PlayerController
			APlayerController* PlayerController = Cast<APlayerController>(Player->GetOwner());
			ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);

			if (LobbyPlayerController)
			{
				UE_LOG(LogTemp, Warning, TEXT("CALLING LOBBYBLASTERCONTROLLER!"));
				LobbyPlayerController->UpdatePlayerListInfo(PlayerNames);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerController is not ALobbyPlayerController or is null."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerState is null."));
		}
	}
}

void ALobbyBlasterGameState::GetPlayersFromGameInstance()
{
	BlasterGameInstance = BlasterGameInstance == nullptr ? Cast<UBlasterGameInstance>(GetGameInstance()) : BlasterGameInstance;
	if (!BlasterGameInstance) return;

	MultiPlayerStates.Empty(); // Clear any previous entries
	PlayerNames.Empty();

	for (APlayerState* Player : BlasterGameInstance->PlayerStates) {
		if (!Player) continue;
		MultiPlayerStates.AddUnique(Player);
		FString PlayerName = Player->GetPlayerName();
		PlayerNames.AddUnique(PlayerName);
	}
}


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
	if (HasAuthority()) // only the server should manage this list
	{
		GetPlayersFromGameInstance();
	}
	BlasterGameInstance = Cast<UBlasterGameInstance>(GetGameInstance());
}

void ALobbyBlasterGameState::OnRep_MultiPlayerStates()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_MultiPlayerStates CALLED - Players in list: %d"), MultiPlayerStates.Num());

	if (MultiPlayerStates.Num() <= 0) return;

	// Defer actual logic to allow time for PlayerStates and ownership to resolve
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALobbyBlasterGameState::UpdateClientPlayerList);
	}
}

void ALobbyBlasterGameState::UpdateClientPlayerList()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_MultiPlayerStates CALLED - Players in list: %d"), MultiPlayerStates.Num());

	if (MultiPlayerStates.Num() <= 0) return;

	for (APlayerState* Player : MultiPlayerStates) {
		/*
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
		*/
		if (!Player)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerState is null during deferred update."));
			continue;
		}

		APlayerController* PlayerController = Cast<APlayerController>(Player->GetOwner());
		if (!PlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerController is null or not yet assigned."));
			continue;
		}

		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);
		if (!LobbyPlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerController is not a LobbyPlayerController."));
			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("Updating player list widget for: %s"), *Player->GetPlayerName());
		LobbyPlayerController->UpdatePlayerListInfo(PlayerNames);
	}
}

void ALobbyBlasterGameState::GetPlayersFromGameInstance()
{
	BlasterGameInstance = BlasterGameInstance == nullptr ? Cast<UBlasterGameInstance>(GetGameInstance()) : BlasterGameInstance;
	if (!BlasterGameInstance) return;

	UE_LOG(LogTemp, Warning, TEXT("GetPlayersFromGameInstance() called on %s"), HasAuthority() ? TEXT("Server") : TEXT("Client"));

	MultiPlayerStates.Empty(); // Clear any previous entries
	PlayerNames.Empty();

	for (APlayerState* Player : BlasterGameInstance->PlayerStates) {
		if (!Player) continue;
		MultiPlayerStates.AddUnique(Player);
		FString PlayerName = Player->GetPlayerName();
		PlayerNames.AddUnique(PlayerName);
	}
}


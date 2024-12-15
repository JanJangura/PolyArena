// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameState.h"
#include "Net/UnrealNetwork.h"
#include "BlasterGame/PlayerController/BlasterPlayerController.h"
#include "BlasterGame/HUD/BlasterHUD.h"
#include "GameFramework/PlayerState.h"
#include "BlasterGame/GameInstance/BlasterGameInstance.h"
#include "BlasterGame/GameMode/LobbyGameMode.h"
#include "BlasterGame/GameMode/BlasterGameMode.h"

void ABlasterGameState::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle PlayerListHandle;
	GetWorld()->GetTimerManager().SetTimer(PlayerListHandle, this, &ABlasterGameState::GetPlayerListFromGameInstance, 1.5f, false);

	BlasterGameMode = Cast<ABlasterGameMode>(GetWorld()->GetAuthGameMode());

	if (BlasterGameMode) {
		BlasterGameMode->OnPlayerTabUpdate.AddDynamic(this, &ABlasterGameState::PlayerTabIsReady);
	}
}

void ABlasterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterGameState, TopScoringPlayers);
	DOREPLIFETIME(ABlasterGameState, MultiBlasterPlayerStates);
}

// This handles the Top Score Players.
void ABlasterGameState::UpdateTopScore(ABlasterPlayerState* ScoringPlayer)
{
	if (TopScoringPlayers.Num() == 0) { // This is if nobody is added into the TopScore, so nobody will have the TopScore at the beginning of the Game. 
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
	else if (ScoringPlayer->GetScore() == TopScore) { // This is if there is a tie between players at the TopScore.
		TopScoringPlayers.AddUnique(ScoringPlayer); // AddUnique allows us to add things into the Array without any duplicates of objects.
	}
	else if (ScoringPlayer->GetScore() > TopScore) { // This is when a Players has beaten the Top Score. 
		TopScoringPlayers.Empty();
		TopScoringPlayers.AddUnique(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();	// This is the new TopScore
	}
}

void ABlasterGameState::AddPlayerToPlayerList(APlayerState* NewPlayerState)
{
	ABlasterPlayerState* BlasterPlayerState = Cast<ABlasterPlayerState>(NewPlayerState);
	if (BlasterPlayerState) {
		MultiBlasterPlayerStates.AddUnique(BlasterPlayerState);

		OnPlayerListUpdate.Broadcast(MultiBlasterPlayerStates);

		ABlasterPlayerController* PlayerController = Cast<ABlasterPlayerController>(BlasterPlayerState->GetPlayerController());
		if (PlayerController) {
			PlayerController->bInProgressState = true;
		}
	}
}

void ABlasterGameState::UpdatePlayerList()
{
	for (ABlasterPlayerState* PS : MultiBlasterPlayerStates) {
		if (PS) {
			ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(PS->GetPlayerController());
			if (BlasterPlayerController && BlasterPlayerController->BlasterHUD) {
				BlasterPlayerController->BlasterHUD->UpdatePlayerList(MultiBlasterPlayerStates);
			}
		}
	}
}

void ABlasterGameState::GetPlayerListFromGameInstance()
{
	BlasterGameInstance = BlasterGameInstance == nullptr ? Cast<UBlasterGameInstance>(GetGameInstance()) : BlasterGameInstance;
	if (!BlasterGameInstance) return;

	if (BlasterGameInstance && BlasterGameInstance->PlayerStates.Num() > 0) {
		for (APlayerState* PlayerStates : BlasterGameInstance->PlayerStates) {
			if (IsValid(PlayerStates)) {
				ABlasterPlayerState* BlasterPlayerState = Cast<ABlasterPlayerState>(PlayerStates);
				if (BlasterPlayerState) {
					MultiBlasterPlayerStates.AddUnique(BlasterPlayerState);
				}
			}			
		}
	}
}

void ABlasterGameState::PlayerTabIsReady(bool bPlayerTabIsReady)
{
	for (ABlasterPlayerState* PS : MultiBlasterPlayerStates) {
		if (PS) {
			ABlasterPlayerController* PlayerController = Cast<ABlasterPlayerController>(PS->GetPlayerController());
			if (PlayerController) {
				PlayerController->bInProgressState = bPlayerTabIsReady;				
			}

		}
	}
}

void ABlasterGameState::OnRep_MultiBlasterPlayerStates()
{
	UpdatePlayerList();
}

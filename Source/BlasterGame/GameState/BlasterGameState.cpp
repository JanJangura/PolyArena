// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameState.h"
#include "Net/UnrealNetwork.h"
#include "BlasterGame/PlayerState/BlasterPlayerState.h"
#include "BlasterGame/PlayerController/BlasterPlayerController.h"
#include "BlasterGame/HUD/BlasterHUD.h"

void ABlasterGameState::BeginPlay()
{
	Super::BeginPlay();
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

void ABlasterGameState::AddPlayerToPlayerList(ABlasterPlayerState* BlasterPlayerState)
{
	if (HasAuthority()) {
		MultiBlasterPlayerStates.AddUnique(BlasterPlayerState);

		ForceNetUpdate();
		UE_LOG(LogTemp, Warning, TEXT("BlasterPlayer Added. Current Count: %d"), MultiBlasterPlayerStates.Num());
	}
}

void ABlasterGameState::UpdatePlayerList()
{
	for (int32 i = 0; i < MultiBlasterPlayerStates.Num(); i++) {
		if (MultiBlasterPlayerStates[i]) {
			ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(MultiBlasterPlayerStates[i]->GetPlayerController());
			if (BlasterPlayerController && IsValid(BlasterPlayerController) && BlasterPlayerController->BlasterHUD) {

				BlasterPlayerController->BlasterHUD->UpdatePlayerList(MultiBlasterPlayerStates);
			}
		}
	}
}

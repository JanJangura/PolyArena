// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameMode.h"
#include "BlasterGame/Character/BlasterCharacter.h"
#include "BlasterGame/PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "BlasterGame/GameInstance/BlasterGameInstance.h"
#include "BlasterGame/PlayerState/BlasterPlayerState.h"
#include "BlasterGame/GameState/BlasterGameState.h"
#include "BlasterGame/HUD/PlayerList.h"
#include "MultiplayerSessionsSubsystem.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

// Remember, this is only happening on the Server.

ABlasterGameMode::ABlasterGameMode()
{
	bDelayedStart = true;	// The Game Mode will stay in the waiting to Start State and it'll actually spawn a default Pawn for all players. This pawn can fly around the level.
}

void ABlasterGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	BlasterGameState = BlasterGameState == nullptr ? GetGameState<ABlasterGameState>() : BlasterGameState;
	if (BlasterGameState) {
		BlasterGameState->AddPlayerToPlayerList(NewPlayer);
	}

	/*
	UBlasterGameInstance* GameInstance = Cast<UBlasterGameInstance>(GetGameInstance());
	if (GameInstance && NewPlayer && NewPlayer->PlayerState) {
		GameInstance->AddPlayerStates(NewPlayer->PlayerState);

		BlasterGameState = BlasterGameState == nullptr ? GetGameState<ABlasterGameState>() : BlasterGameState;
		if (BlasterGameState) {
			BlasterGameState->AddPlayerToPlayerList(NewPlayer->PlayerState);
			UE_LOG(LogTemp, Warning, TEXT("Added player to GameState: %s"), *NewPlayer->PlayerState->GetPlayerName());
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Failed to add player to GameState in PostLogin."));
		}
	}
	*/
}

void ABlasterGameMode::BeginPlay()
{
	Super::BeginPlay();
	BlasterGameState = GetGameState<ABlasterGameState>();
	LevelStartingTime = GetWorld()->GetTimeSeconds();
	OnPlayerTabUpdate.Broadcast(false);

	/*
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->PlayerState)
		{
			if (BlasterGameState)
			{
				BlasterGameState->AddPlayerToPlayerList(PC->PlayerState);
			}
		}
	}
	*/
}

void ABlasterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart) {
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.1f) {
			StartMatch();	// This function is inherited within the GameMode Class.
			OnPlayerTabUpdate.Broadcast(true); UE_LOG(LogTemp, Warning, TEXT("OnPlayerTabUpdate.Broadcast(true)"));
		}
	}
	else if (MatchState == MatchState::InProgress) {
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f) {
			SetMatchState(MatchState::Cooldown); // This is how we'll set the MatchState to Cooldown.
		}
	}
	else if (MatchState == MatchState::Cooldown) {
		CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		
		if (CountdownTime <= 0.f) {
			EndGame();	// This function is inherited within the GameMode Class.
		}
	}
}

void ABlasterGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	// This iterator allows us to loop through all PlayerControllers that exist in the game. 
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
		ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayerController) {
			BlasterPlayerController->OnMatchStateSet(MatchState);
			BlasterPlayerController->CastBlasterHUD();
		}
	}
}

void ABlasterGameMode::EndGame()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance) {

		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();

		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ABlasterGameMode::OnDestroyedSession);

			MultiplayerSessionsSubsystem->DestroySession();
		}
	}
}

void ABlasterGameMode::OnDestroyedSession(bool bWasSuccessful)
{
	if (bWasSuccessful) {
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
			APlayerController* PC = It->Get();
			if (PC) {
				PC->ClientTravel("Game/Scenes/PolyArenaMainMenu", ETravelType::TRAVEL_Absolute);
			}
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Session destruction failed"));
	}
}

void ABlasterGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	if (AttackerController == nullptr || AttackerController->PlayerState == nullptr) return;
	if (VictimController == nullptr || VictimController->PlayerState == nullptr) return;

	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	ABlasterPlayerState* VictimPlayerState = VictimController ? Cast<ABlasterPlayerState>(VictimController->PlayerState) : nullptr;
	BlasterGameState = BlasterGameState == nullptr ? GetGameState<ABlasterGameState>() : BlasterGameState;

	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState && BlasterGameState) {
		AttackerPlayerState->AddToScore(1.f);
		BlasterGameState->UpdateTopScore(AttackerPlayerState);

		/*if (AttackerPlayerState->GetScore() == MaxKillScore) {
			SetMatchState(MatchState::Cooldown);
		}*/
	}

	if (VictimPlayerState && VictimPlayerState != AttackerPlayerState) {
		VictimPlayerState->AddToDefeats(1);
	}

	if (ElimmedCharacter) {
		ElimmedCharacter->Elim(false);
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
		ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayer && AttackerPlayerState && VictimPlayerState) {
			BlasterPlayer->BroadCastElim(AttackerPlayerState, VictimPlayerState);
		}
	}
}

void ABlasterGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter) {

		// Reset is an inherited function and it's on the Pawn Class. So what it does is it detatches the character from the controller and calls on Unposses().
		ElimmedCharacter->Reset(); 

		// Destroy the Character.
		ElimmedCharacter->Destroy(); 
	}
	if (ElimmedController) {
		
		// This is a function that the Game Mode inherits called "RestartPlayerAtPlayerStart()", there are multiple functions for restarting a Player, but 
		// "RestartPlayerAtPlayerStart" takes an actor, which it uses as a Player Start and it spawns a new character at the location of that Start Spot.
		// So RestartPlayerAtPlayerStart can take any actor and it'll spawn the character at that start spot, but we would usually use a PlayerStart for this found in Editor.
		// We should actually get a Player Start that exists in the world at random.

		// This is an Array of Type AActor that is filled with Player Starts.
		TArray<AActor*> PlayerStart;

		// This is how we would get all actors of the Player Start Class that exist in our world. We can do this with "APlayerStart::StaticClass()"
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStart);

		// This how we would select a random Player Start, Actors.Num() - 1 represents the Index within our Actors Array.
		int32 Selection = FMath::RandRange(0, PlayerStart.Num() - 1);

		// First Input is the Controller and it's going to possess the new player that will be spawned, Second Input is the Actor's Player Start which we'll declare randomly.
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStart[Selection]);
	}
}

void ABlasterGameMode::PlayerLeftGame(ABlasterPlayerState* PlayerLeaving)
{
	if (PlayerLeaving == nullptr) return;

	BlasterGameState = BlasterGameState == nullptr ? GetGameState<ABlasterGameState>() : BlasterGameState;
	if (BlasterGameState && BlasterGameState->TopScoringPlayers.Contains(PlayerLeaving)) {
		BlasterGameState->TopScoringPlayers.Remove(PlayerLeaving);
	}

	// Get the pawn of the PlayerLeaving State
	ABlasterCharacter* CharacterLeaving = Cast<ABlasterCharacter>(PlayerLeaving->GetPawn());
	if (CharacterLeaving) {
		CharacterLeaving->Elim(true);
	}
}
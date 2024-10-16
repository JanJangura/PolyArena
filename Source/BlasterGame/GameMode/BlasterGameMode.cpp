// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameMode.h"
#include "BlasterGame/Character/BlasterCharacter.h"
#include "BlasterGame/PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "BlasterGame/PlayerState/BlasterPlayerState.h"
#include "BlasterGame/GameState/BlasterGameState.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

// Remember, this is only happening on the Server.

ABlasterGameMode::ABlasterGameMode()
{
	bDelayedStart = true;	// The Game Mode will stay in the waiting to Start State and it'll actually spawn a default Pawn for all players. This pawn can fly around the level.
}

void ABlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void ABlasterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart) {
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f) {
			StartMatch();	// This function is inherited within the GameMode Class.
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
			RestartGame();	// This function is inherited within the GameMode Class.
		}
	}
}

void ABlasterGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	// This iterator allows us to loop through all PlayerControllers that exist in the game. 
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
		ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayer) {
			BlasterPlayer->OnMatchStateSet(MatchState);
			BlasterPlayer->CastBlasterHUD();
		}
	}
}

void ABlasterGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	ABlasterPlayerState* VictimPlayerState = VictimController ? Cast<ABlasterPlayerState>(VictimController->PlayerState) : nullptr;
	ABlasterGameState* BlasterGameState = GetGameState<ABlasterGameState>();

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
		ElimmedCharacter->Elim();
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



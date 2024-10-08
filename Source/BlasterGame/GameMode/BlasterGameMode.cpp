// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameMode.h"
#include "BlasterGame/Character/BlasterCharacter.h"
#include "BlasterGame/PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

// Remember, this is only happening on the Server.

void ABlasterGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
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

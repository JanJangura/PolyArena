// Fill out your copyright notice in the Description page of Project Settings.

// This LobbyGameMode is needed to see how many players have connected to the lobby level or traveled to it. Then once a certain number of players have arrived,
// we can then transfer all players to the actual game map. 
 
#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	// PostLogin() is the first place where it's safe to access the player's controller that has just joined the game. 
	Super::PostLogin(NewPlayer);

	// GameState is a class. The GameMode has a variable called Game State which is a TObjectPtr<AGameStateBase>. This ptr provides access tracking, which
	// means you can actually detect when the object is being used. This PlayerArray contains a player state for each player who's joined the game and it being a TArray,
	// it has a Num() function that returns the number of elements within the array as an int32.
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 2) {	// We'll check to see if the amount of players have been met, so we can travel to the actual game when it's met.
		// We'll call ServerTravel() so all connecting players from the lobby, can travel to the Actual Game. Remember, ServerTravel() is a function that belongs to UWorld.
		UWorld* World = GetWorld();

		// We want to travel Seamlessly to the Game Level. So we'll use the boolean variable called "bUseSeamlessTravel" that exists on the GameMode class and set that to true.
		bUseSeamlessTravel = true;

		// ServerTravel() takes in an address to travel to. The server only ever exists on the GameMode, so when we call this function, we can travel to the level we want to. 
		// Then all connected clients will travel to that level. In parameters, ServerTravel() takes an FString, then we can use the syntax "?" to add additional options to 
		// the address. In this case, the option we need is "listen" because we want to dedicate this BlasterMap to be opened as a listen server for clients to connect to. 
		World->ServerTravel(FString("/Game/Scenes/BlasterMap?listen"));
	}


}
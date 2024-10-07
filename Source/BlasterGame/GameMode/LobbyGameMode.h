// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
public: 
	UFUNCTION(BlueprintCallable)
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable)
	void ServerJumpToGame(int32 TotalMaxPlayers);

	// If the LaunchGameButton is clicked, then this needs to be true.
	bool LaunchGameButtonClicked = false;

	// Lobby Settings
	int32 CurrentNumOfPlayers;
	int32 MaxPlayers{ 10 };
};

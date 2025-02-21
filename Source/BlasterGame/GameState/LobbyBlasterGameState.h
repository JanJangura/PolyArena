// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Delegates/DelegateCombinations.h"
#include "LobbyBlasterGameState.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API ALobbyBlasterGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	void GetPlayersFromGameInstance();

	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_MultiPlayerStates)
	TArray<class APlayerState*> MultiPlayerStates;

private:
	class UBlasterGameInstance* BlasterGameInstance;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_MultiPlayerStates();

	TArray<FString> PlayerNames;
};

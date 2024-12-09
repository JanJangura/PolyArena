// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BlasterGameState.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API ABlasterGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void UpdateTopScore(class ABlasterPlayerState* ScoringPlayer); // This is how we'll enter the top scoring players into the Array. 

	void AddPlayerToPlayerList(ABlasterPlayerState* BlasterPlayerState);

	UPROPERTY(Replicated)
	TArray<ABlasterPlayerState*> TopScoringPlayers;

	UPROPERTY(Replicated)
	TArray<ABlasterPlayerState*> MultiBlasterPlayerStates;

	void UpdatePlayerList();

private:
	float TopScore = 0.f;
};

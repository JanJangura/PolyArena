// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Delegates/DelegateCombinations.h" // For declaring and using delegates
#include "BlasterGame/PlayerState/BlasterPlayerState.h"
#include "BlasterGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerListUpdate, const TArray<ABlasterPlayerState*>&, BlasterPlayerStates);
/**
 * 
 */
UCLASS()
class BLASTERGAME_API ABlasterGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	class ABlasterGameMode* BlasterGameMode;

	FOnPlayerListUpdate OnPlayerListUpdate;

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void UpdateTopScore(class ABlasterPlayerState* ScoringPlayer); // This is how we'll enter the top scoring players into the Array. 

	void AddPlayerToPlayerList(APlayerState* NewPlayerState);

	UPROPERTY(Replicated)
	TArray<ABlasterPlayerState*> TopScoringPlayers;

	UPROPERTY(ReplicatedUsing = OnRep_MultiBlasterPlayerStates)
	TArray<ABlasterPlayerState*> MultiBlasterPlayerStates;

	void UpdatePlayerList();

	void GetPlayerListFromGameInstance();

	bool bInProgressState = false;

	UFUNCTION()
	void PlayerTabIsReady(bool bPlayerTabIsReady);

private:
	float TopScore = 0.f;
	
	class UBlasterGameInstance* BlasterGameInstance;

	UFUNCTION()
	void OnRep_MultiBlasterPlayerStates();
};

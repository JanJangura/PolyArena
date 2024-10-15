// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BlasterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API ABlasterPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	// Replication Notifies
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void OnRep_Score() override;

	UFUNCTION()
	virtual void OnRep_Defeats();

	// This Score will be updated on the Server.
	void AddToScore(float ScoreAmount);

	// This Defeats will be updated on the Server
	void AddToDefeats(int32 DefeatsAmount);

private:
	// This UPROPERTY() Macro will initialize these classes to a nullptr, so that they aren't some garbage memory type that can still pass in our functions when validating if it's a
	// nullptr or not. Another way to make sure we avoid garbage memory for these Characters is to just set them equal to nullptr.
	UPROPERTY()
	class ABlasterCharacter* Character;
	UPROPERTY()
	class ABlasterPlayerController* Controller;

	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
	int32 Defeats; // This is our Deaths & Scores is our Kills.
};

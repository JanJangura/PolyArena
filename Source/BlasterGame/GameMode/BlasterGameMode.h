// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Delegates/DelegateCombinations.h"
#include "BlasterGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerTabUpdate, bool, bPlayerTabIsReady);

namespace MatchState
{
	extern BLASTERGAME_API const FName Cooldown; // Match Duration has been reached. Display winner and begin cooldown timer. 
}

/**
 * 
 */
UCLASS()
class BLASTERGAME_API ABlasterGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	FOnPlayerTabUpdate OnPlayerTabUpdate;

	ABlasterGameMode();
	virtual void Tick(float DeltaTime) override;
	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController);
	virtual void RequestRespawn(class ACharacter* ElimmedCharacter, AController* ElimmedController);

	void PlayerLeftGame(class ABlasterPlayerState* PlayerLeaving);

	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;

	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 15.f;

	float LevelStartingTime = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float MaxKillScore = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AmmoRespawnCooldownTime = 7.f;

	UFUNCTION(BlueprintCallable)
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION()
	void OnDestroyedSession(bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnSpecificAmmo(FTransform AmmoLocation, class AActor* Ammo);

protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;

private:
	float CountdownTime = 0.f;
	class ABlasterGameState* BlasterGameState;

	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	void EndGame();

public:
	FORCEINLINE float GetCountdownTime() const { return CountdownTime; }
};

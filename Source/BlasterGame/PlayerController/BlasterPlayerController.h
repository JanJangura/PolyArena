// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterGame/Weapon/WeaponTypes.h"
#include "BlasterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABlasterPlayerController();
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDScore(float Score);	// The variable Score in the Player State is a float by default, so we're just matching it here.
	void SetHUDDefeats(int32 Defeats);
	void SetHUDCarriedAmmo(int32 Ammo);
	void SetHUDMatchCountdown(float CountdownTime);
	void SetHUDAnnouncementCountdown(float CountdownTime);
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetHUDHealth(float Health, float MaxHealth);
	virtual void OnPossess(APawn* InPawn) override;
	virtual float GetServerTime(); // Synched with Server World Clock.
	virtual void ReceivedPlayer() override; // Synched with Server World Clock as soon as possible.
	void OnMatchStateSet(FName State);
	void HandleMatchHasStarted();
	void HandleCooldown();
	void CastBlasterHUD();
	void UpdateWeaponIcon(EWeaponType WeaponType);

protected:
	virtual void BeginPlay() override;
	void SetHUDTime();

	// ***** Sync Time between Client and Server *****
	// Server's RPC - Requests the Current Server time, passing in the Client's time when the request was sent. 
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest); 

	// Client's RPC - Reports the Current Server time to the Client in response to ServerRequestServerTime.
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeOfServerReceivedCientRequest); // Reports 

	// Difference between Client and Server Time. 
	float ClientServerDelta = 0.f;

	// So every 5 seconds, we'll synch with the Server Time again.
	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.f;	

	// Update this every frame in the Tick() function, and reset it everytime we synch up with the Server.
	float TimeSyncRunningTime = 0.f;
	void CheckTimeSynch(float DeltaTime);

	void PollInit();

	UFUNCTION(Server,Reliable)
	void ServerCheckMatchState();

	UFUNCTION(Client, Reliable)	// This will handle informing the Client of the Match State when they join. 
	void ClientJoinMidGame(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime);

private:
	UPROPERTY()
	class ABlasterHUD* BlasterHUD;

	UPROPERTY()
	class ABlasterGameMode* BlasterGameMode;

	class ABlasterCharacter* Character;

	AHUD* CurrentHUD;

	float LevelStartingTime = 0.f;
	float MatchTime = 0.f;
	float WarmupTime = 0.f;
	float CooldownTime = 0.f;
	uint32 CountdownInt = 0; 

	// We need this to be replicated so all Clients can have access to this.
	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	bool bInitializeCharacterOverlay = false;

	float HUDHealth;
	float HUDMaxHealth;
	float HUDScore;	// Eliminated
	int32 HUDDefeats; // Deaths
	float HUDCarriedAmmo;
	bool bInitializeCarriedAmmo = false;
	float HUDWeaponAmmo;
	bool bInitializeWeaponAmmo = false;
	bool bInitializeWeaponIcon = false;
};

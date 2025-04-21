// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerState.h"
#include "BlasterGame/Character/BlasterCharacter.h"
#include "BlasterGame/PlayerController/BlasterPlayerController.h"
#include "BlasterGame/GameState/BlasterGameState.h"
#include "Net/UnrealNetwork.h"

// We need this for Replication.
void ABlasterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterPlayerState, Defeats);
}

// This function should only be called on the Server.
void ABlasterPlayerState::AddToScore(float ScoreAmount)
{
	// Score is inherited within the Player State.
	SetScore(GetScore() + ScoreAmount);

	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if (Character) {

		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if (Controller) {
			// We'll update the HUD here.
			Controller->SetHUDScore(GetScore());
		}
	}

	UpdatePlayerList();
}

// This replicated function is called on the Clients.
void ABlasterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	//GetPawn();	// Return the Pawn associated with this Player State.
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if (Character) {

		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if (Controller) {
			// We'll update the HUD here.
			Controller->SetHUDScore(GetScore());
			Controller->UpdatePlayerList();
		}
	}
}

// This function should only be called on the Server.
void ABlasterPlayerState::AddToDefeats(int32 DefeatsAmount)
{
	Defeats += DefeatsAmount;

	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if (Character) {

		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if (Controller) {
			// We'll update the HUD here.
			Controller->SetHUDDefeats(Defeats);
		}
	}

	UpdatePlayerList();
}

void ABlasterPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ABlasterPlayerState::UpdatePlayerList()
{
	BlasterGameState = BlasterGameState == nullptr ? GetWorld()->GetGameState<ABlasterGameState>() : BlasterGameState;
	if (BlasterGameState)
	{
		BlasterGameState->Multicast_UpdatePlayerList();
	}
}

// This replicated function is called on the Clients.
void ABlasterPlayerState::OnRep_Defeats()
{
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if (Character) {

		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if (Controller) {
			// We'll update the HUD here.
			Controller->SetHUDDefeats(Defeats);
			Controller->UpdatePlayerList();
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerState.h"
#include "BlasterGame/Character/BlasterCharacter.h"
#include "BlasterGame/PlayerController/BlasterPlayerController.h"

void ABlasterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	GetPawn();	// Return the Pawn associated with this Player State.
}

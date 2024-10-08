// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameMode.h"
#include "BlasterGame/Character/BlasterCharacter.h"
#include "BlasterGame/PlayerController/BlasterPlayerController.h"

// Remember, this is only happening on the Server.

void ABlasterGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	if (ElimmedCharacter) {
		ElimmedCharacter->Elim();
	}
}

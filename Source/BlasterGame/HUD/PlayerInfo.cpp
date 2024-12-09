// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfo.h"
#include "Components/TextBlock.h"

void UPlayerInfo::SetPlayerInformation(FString GetPlayerName, int32 GetKillScore, int32 GetDeathScore)
{
	if (PlayerName && KillScore && DeathScore) {
		FText Kills = FText::AsNumber(GetKillScore);
		FText Deaths = FText::AsNumber(GetDeathScore);

		PlayerName->SetText(FText::FromString(GetPlayerName));
		KillScore->SetText(Kills);
		DeathScore->SetText(Deaths);
	}
}
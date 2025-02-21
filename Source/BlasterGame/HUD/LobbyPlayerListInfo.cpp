// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerListInfo.h"
#include "Components/TextBlock.h"

void ULobbyPlayerListInfo::UpdatePlayerName(FString Player)
{
	if (PlayerName) {
		PlayerName->SetText(FText::FromString(Player));
	}
}

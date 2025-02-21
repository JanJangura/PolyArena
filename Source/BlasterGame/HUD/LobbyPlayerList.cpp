// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerList.h"
#include "Components/VerticalBox.h"
#include "BlasterGame/HUD/LobbyPlayerListInfo.h"

void ULobbyPlayerList::AddPlayerInfoWidget(FString PlayerName)
{
	if (LobbyPlayerInfoWidget && PlayerListVerticalBox) {
		ULobbyPlayerListInfo* NewPlayerInfo = CreateWidget<ULobbyPlayerListInfo>(this, LobbyPlayerInfoWidget);

		if (NewPlayerInfo) {
			NewPlayerInfo->UpdatePlayerName(PlayerName);
			PlayerListVerticalBox->AddChild(NewPlayerInfo);
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerList.h"
#include "BlasterGame/PlayerState/BlasterPlayerState.h"
#include "BlasterGame/PlayerController/BlasterPlayerController.h"
#include "PlayerInfo.h"
#include "Components/ScrollBox.h"
#include "Components/Widget.h"
#include "BlasterGame/GameState/BlasterGameState.h"


void UPlayerList::PlayerListSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
}

void UPlayerList::PlayerListTearDown()
{
	SetVisibility(ESlateVisibility::Hidden);
	RemoveFromParent();
}

void UPlayerList::AddPlayerInfoWidget(FString PlayerName, int32 KillScore, int32 DeathScore)
{
	if (PlayerInfoWidget && PlayerScrollBox) {
		UPlayerInfo* NewPlayerInfo = CreateWidget<UPlayerInfo>(this, PlayerInfoWidget);

		if (NewPlayerInfo) {
			NewPlayerInfo->SetPlayerInformation(PlayerName, KillScore, DeathScore);
			PlayerScrollBox->AddChild(NewPlayerInfo);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerListInfo.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API ULobbyPlayerListInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerName;

	void UpdatePlayerName(FString Player);
};

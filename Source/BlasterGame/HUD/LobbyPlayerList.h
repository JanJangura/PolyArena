// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerList.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API ULobbyPlayerList : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void AddPlayerInfoWidget(FString PlayerName);

private:
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> LobbyPlayerInfoWidget;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* PlayerListVerticalBox;
};

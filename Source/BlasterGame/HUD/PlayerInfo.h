// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfo.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API UPlayerInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KillScore;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DeathScore;

	void SetPlayerInformation(FString GetPlayerName, int32 GetKillScore, int32 GetDeathScore);
};

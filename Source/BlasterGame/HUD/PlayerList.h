// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerList.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API UPlayerList : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void PlayerListSetup();
	void PlayerListTearDown();

	UPROPERTY(BlueprintReadOnly)
	TArray<class UPlayerInfo*> MultiPlayerInfo;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* PlayerScrollBox;

	void AddPlayerInfoWidget(FString PlayerName, int32 KillScore, int32 DeathScore);

private:
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> PlayerInfoWidget;
};

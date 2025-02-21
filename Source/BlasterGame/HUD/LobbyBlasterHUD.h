// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterGame/HUD/BlasterHUD.h"
#include "LobbyBlasterHUD.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API ALobbyBlasterHUD : public ABlasterHUD
{
	GENERATED_BODY()

public:
	ALobbyBlasterHUD();

private:
	void AddPauseUI();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void ShowPlayerList();
	class ALobbyBlasterGameState* LobbyBlasterGameState;

public:

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UUserWidget> LaunchGameButtonClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UUserWidget> LoadingClass;

	class ULaunchGameButton* LaunchGameButton;
	class ULoading* LoadingText;

	void ToggleLaunchGameButton();
	void HideLaunchGameButton();
	void ShowLaunchGameButton();
	void ShowLoadingText();

	// Players List
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UUserWidget> LobbyPlayerListClass;

	class ULobbyPlayerList* LobbyPlayerList;

	void UpdatePlayerList(FString PlayerName);
};

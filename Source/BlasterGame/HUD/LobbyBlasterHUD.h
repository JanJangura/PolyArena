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

public:

	UPROPERTY(EditAnywhere, Category = "Launch Game")
	TSubclassOf<class UUserWidget> LaunchGameButtonClass;

	class ULaunchGameButton* LaunchGameButton;

	void ToggleLaunchGameButton();
	void HideLaunchGameButton();
	void ShowLaunchGameButton();
};

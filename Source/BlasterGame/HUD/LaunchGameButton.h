// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LaunchGameButton.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API ULaunchGameButton : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

private:
	
	UPROPERTY(meta = (BindWidget))	// This means that the button Widget on our blueprint will be linked to our button variable in C++.
	class UButton* LaunchGameButton;

	UFUNCTION()
	void LaunchButtonClicked();
	
	void LobbyTearDown();

protected:
	UFUNCTION(NetMulticast, Reliable)
	void ShowLoadingScreen(class ALobbyBlasterHUD* LobbyBlasterHUD);
};

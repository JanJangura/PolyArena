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

private:
	UPROPERTY(meta = (BindWidget))	// This means that the button Widget on our blueprint will be linked to our button variable in C++.
	class UButton* LaunchGameButton;

	void LaunchButtonClicked();

protected:
	virtual bool Initialize() override; // This function is what the Menu Class Inherits. 
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))	// This meta = (BindWidget) is needed to bind this variable to the progress bar within our CharacterOverlayWidget
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreAmount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DefeatsAmount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponAmmoAmount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CarriedAmmoAmount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MatchCountdownText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponSelectionText;
};

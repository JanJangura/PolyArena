// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"

#include "SessionEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API USessionEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SessionNameText;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinSessionButton;

	// Store the session data to use when the join button is clicked
	FOnlineSessionSearchResult SessionResult;

	void Setup(const FOnlineSessionSearchResult& Result);

	UFUNCTION()
	void OnJoinSessionButtonClicked();
};

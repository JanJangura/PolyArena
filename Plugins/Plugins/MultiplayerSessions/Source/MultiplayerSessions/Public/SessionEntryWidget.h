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
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* SessionNameText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* SessionMatchType;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* SessionPlayers;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* SessionMapType;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* JoinSessionButton;

	// Store the session data to use when the join button is clicked
	FOnlineSessionSearchResult SessionResult;

	void Setup(const FOnlineSessionSearchResult& Result);

	UFUNCTION()
	void OnJoinSessionButtonClicked();

	UPROPERTY(BlueprintReadOnly)
	FString GetSessionName;

	UFUNCTION(BlueprintCallable)
	void SessionSetup(FString GetSessionID, FString GetSessionUser, FString GetMaxPlayers, int32 SessionIndex, FString SelectedMapType, class UMenu* Menu);

	UPROPERTY(BlueprintReadOnly)
	UMenu* MenuRef;

	int SessionIndexRef;

protected:

	virtual bool Initialize() override;
};

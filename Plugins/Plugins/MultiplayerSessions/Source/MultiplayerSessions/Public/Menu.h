// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "Menu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/ThirdPerson/Maps/Lobby?listen"))); // We've set default values to these passed in parameters. 

protected:
	virtual bool Initialize() override; // This function is what the Menu Class Inherits. 
	virtual void NativeDestruct(); // Menu Class also inherits this function. This gets called when the level is removed 
																			// from the world when we travel to another level. This means that if we're traveling 
																			// to another level, the current level will be destroyed and removed. Then all user widgets 
																			// will call this function. We'll be calling MenuTearDown() here.

	///////////* CUSTOM DELEGATE CALLBACKS */////////
	// Callbacks for the Custom Delegates on the MultiplayerSessionsSubsystem.
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);

	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSession(bool bWasSucessful);

private:

	UPROPERTY(meta = (BindWidget))	// This means that the button Widget on our blueprint will be linked to our button variable in C++.
	class UButton* Host_Button;

	UPROPERTY(meta = (BindWidget))
	UButton* Join_Button;

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	// This functions allows us to take the Menu Settings Down.
	void MenuTearDown();

	// The subsystem designed to handle all online session functionality.
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	// Match Settings
	int32 NumPublicConnections{4};
	FString MatchType{TEXT("FreeForAll")};
	FString PathToLobby{ TEXT("") };
};

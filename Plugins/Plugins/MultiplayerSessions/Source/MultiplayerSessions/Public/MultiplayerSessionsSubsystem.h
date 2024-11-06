// Fill out your copyright notice in the Description page of Project Settings.

// This our own custom Subsystem
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

#include "MultiplayerSessionsSubsystem.generated.h"

///////////* CREATING CUSTOM DELEGATES */////////
// Declaring our own custom delegates for the Menu Class to bind callbacks to. 

// Declaring a Multicast Delegate with parameters and make it capable of binding a function that takes a single parameter.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful); 

// Delegate for Finding Sessions
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);

// Delegate for joining Sessions
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);

// Delegate for Destroy Session
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);

// Delegate for On Start Session
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UMultiplayerSessionsSubsystem();

	///////////* MENU FUNCTIONALITIES */////////
	// To handle Session Functionality, the Menu Class will call these functions.
	void CreateSession(int32 NumPublicConnections, FString MatchType, FName SessionName);
	void FindSessions(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);
	void DestroySession();
	void StartSession();

	///////////* DECLARING CUSTOM DELEGATES */////////// 
	// Our own custom Delegates for the Menu class to bind callbacks to.
	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete; 
	FMultiplayerOnFindSessionsComplete MultiplayerOnFindSessionsComplete;
	FMultiplayerOnJoinSessionComplete MultiplayerOnJoinSessionComplete;
	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;
	FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;

protected:

	///////////* CALLBACKS */////////
	// Internal Callbacks for the delegates, we'll add to the Online Session Interface delegate list.
	// These don't need to be called outside this class.
	// These are internally used by our subsystem to interact with the Online Session Interface.
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

private:
	///////////* POINTERS */////////
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;		// We'll always store our last session settings that we created.
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;			// This is to check and see the last session search.

	///////////* DELEGATES */////////
	// These are our Delegates to add to the Online Session Interface Delegate List.
	// We'll bind our MultiplayerSessionsSubsystem internal callbacks to these.
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;

	///////////* DELEGATE HANDLES */////////
	// These are our Delegate Handles of type "FDelegateHandle".
	// Each Delegate Handle has been create for each delegate, and we'll be using these Handles to remove Delegates when we no longer need them.
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	bool bCreateSessionOnDestroy{ false };
	int32 LastNumPublicConnections;
	FString LastMatchType;
	FName LastSessionName;
};

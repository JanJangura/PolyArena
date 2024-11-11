// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem():
	// Initializing in the Constructor's initializing list
	// Binding all Callbacks that we created, onto all Delegates that we defined from the header file (in the Constructor). 
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
	// This is how we obtain the Subsystem.
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if (Subsystem) {
		// This is how we get our Session Interface.
		SessionInterface = Subsystem->GetSessionInterface();
	}
}

/////////////////////////////////////////////////////
///////////////* MENU FUNCTIONALITIES */////////////
/////////////////////////////////////////////////////
void UMultiplayerSessionsSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType, FName SessionName)
{
	DesiredNumPublicConnections = NumPublicConnections;
	DesiredMatchType = MatchType;
	DesiredSessionName = SessionName;

	if (!SessionInterface.IsValid()) {
		return;
	}

	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr) {
		bCreateSessionOnDestroy = true;
		LastNumPublicConnections = NumPublicConnections;
		LastMatchType = MatchType;
		LastSessionName = SessionName;

		DestroySession();
	}

	// Store the Delegate in a FDelegate Handle so we can later remove it from the Delegate List. 
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowJoinInProgress = true;		// This is so we can allow players to join on going sessions.
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bUseLobbiesIfAvailable = true;
	LastSessionSettings->BuildUniqueId = 1; // This allows us to have multiple users launching and hosting their own builds, which also allows other players to join them.
	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->Set(FName("SessionName"), SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->Set(FName("MaxPlayers"), FString::FromInt(NumPublicConnections), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	
	// This essentially checks if we failed to create a session.
	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings)) { // We're checking to see if Create Session returns true.
		// If this returns false, remove this Create Session Complete Delegate from the Online Interface Delegate List using our Delegate Handle. 
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		// Broadcast our own custom Delegate back to the Menu. This will Broadcast our Delegate and pass in a value that we'd like our callback function to recieve.
		MultiplayerOnCreateSessionComplete.Broadcast(false);  // In this case, the menu knows that the Create Session failed.							
	}
}

void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
	if (!SessionInterface.IsValid()) {
		return;
	}

	// This is not our custom Delegate, this is the Delegate for the Session Interface Delegate List (Private Section in Header File). This returns a Delegate Handle.
	// Because it returns a Delegate Handle we can then store it in our custom made Delegate Handle called "FindSessionsCompleteDelegateHandle" for finding sessions.
	FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	// LastSessionSearch is a shared pointer that will hold the search object. 
	LastSessionSearch = MakeShareable(new FOnlineSessionSearch()); // This creates a new instance of FOnlineSessionSearch and wraps it in a TSharedPtr so Unreal can manage it.
	LastSessionSearch->MaxSearchResults = MaxSearchResults; // This sets the maximum number of search results.

	// IOnlineSubsystem::Get()->Subsystem checks if the current online subsystem is NULL, which means there is no real online subsystem being used, so the game is 
	// running without an online service (LAN-ONLY Mode). If the IOnlineSubsystem::Get()->Subsystem returns a NULL, then bIsLanQuery is set to true. 
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// This Handles when Finding Sessions has failed.
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef())) { // This entire Line, returns a bool.

		// We are now clearing this Delegate from the Delegate List. 
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		// We are now broadcasting our own custom Delegate. Now our menu will recieve this broadcast as false and that TArray will be empty.
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false); // Passing in an Empty array, because Search Result is empty, and a false bool.
	}
}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if (!SessionInterface.IsValid()) {
		// We'll broadcast back to the Menu.
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	// Call Join Session on the Session Interface, which means we want to add our Delegate to its Delegate list and store in a Delegate Handle.
	// We'll pass in the Session Interface Delegate list and then store it all into a Delegate Handle. So now we've added the delegate to the session interface delegate list.
	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate); 

	// Now we can call the Steam's JoinSession() function.
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult)) {
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UMultiplayerSessionsSubsystem::DestroySession()
{
	if (!SessionInterface.IsValid()) {
		MultiplayerOnDestroySessionComplete.Broadcast(false);
		return;
	}

	DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	if (!SessionInterface->DestroySession(NAME_GameSession)) {
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
		MultiplayerOnDestroySessionComplete.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::StartSession()
{
}

/////////////////////////////////////////////////////
/////////////////////* CALLBACKS *///////////////////
/////////////////////////////////////////////////////
void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful){ // This is fired off when the session interface 
	if (SessionInterface) {																			 // loops through its delegate list and fires off these callbacks.
		// We'll clear the Delegate into two different locations. 
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	} 

	// Access our Delegate, then Broadcast of our Delegate is set to what's passed in parameters once Create Session is completed. This will let the Menu know 
	// if our Create Session Callback is successful. 
	MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful); 
}

void UMultiplayerSessionsSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	if (SessionInterface) {
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}
	
	// So if we do get some search results, but the TArray is empty, we need to let the menu knows that we haven't found any valid search results.
	// So if the SearchResults (which is a TArray) is empty, then we'll broadcast false to the menu.
	if (LastSessionSearch->SearchResults.Num() <= 0) {
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	// Access our Delegate, then Broadcast to our Menu using our custom Delegate. Within the Broadcast, we can pass in an array of search results.
	MultiplayerOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface) {
		// Clear our Delegate.
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	// Broadcast the Result to the Menu.
	MultiplayerOnJoinSessionComplete.Broadcast(Result);
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface) {
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}
	if (bWasSuccessful && bCreateSessionOnDestroy) {
		bCreateSessionOnDestroy = false;
		CreateSession(LastNumPublicConnections, LastMatchType, LastSessionName);
	}
	MultiplayerOnDestroySessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}

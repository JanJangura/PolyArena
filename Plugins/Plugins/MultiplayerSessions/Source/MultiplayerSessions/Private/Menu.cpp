// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Components/ScrollBox.h"
#include "SessionEntryWidget.h"

void UMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	// Initializing the variables here
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	NumberOfPublicConnections = NumberOfPublicConnections;
	TypeOfMatch = TypeOfMatch;

	AddToViewport(); // This adds the Widget that we created into our Player's Viewport.
	SetVisibility(ESlateVisibility::Visible); // This sets the visibility of our Widget Menu buttons so Players can see them. This is an enum value. 
	this->SetIsFocusable(true); // This means that the widget can recieve keyboard input, which allows players to interact with the UI elements. 

	UWorld* World = GetWorld(); // We'll create a World of type UWorld, then the function GetWorld() allows us to retrieve current game world instances. 
	if (World) {
		APlayerController* PlayerController = World->GetFirstPlayerController(); // We'll use the World to access the first player's controller.
		if (PlayerController) { // We'll check if the PlayerController is valid.
			FInputModeUIOnly InputModeData; // We'll create InputModeData of FInputModeUIOnly type which is a struct that is used to configure the input
											// mode for player controllers that focus on UI Elements. 
			InputModeData.SetWidgetToFocus(TakeWidget()); // We'll set the Widget to focus, then the TakeWidget() function returns the widget that should recieve input focus. 
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // This allows to control the mouse cursor behavior, but we set it to DoNotLock for movement.
			PlayerController->SetInputMode(InputModeData); // This sets the input mode to InputModeData that we provided, so player controller can only use this UI. 
			PlayerController->SetShowMouseCursor(true); // This function allows us to see the mouse cursor if true is passed in parameters. 
		}
	}

	UGameInstance* GameInstance = GetGameInstance(); // We'll use the GameInstance() to obtain our custom made MultiplayerSessionsSubsystem Script
	if (GameInstance) {
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>(); // GameInstance has a function to retrieve our SubSystem.
	}

	// Binding our callbacks to our Custom Delegates
	if (MultiplayerSessionsSubsystem) { // If the Subsystem is valid, then we'll bind our callback to our Custom Delegate.
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession); // Add Dynamic takes a user object and a function name. 
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions); // This is not a Dynamic Delegate, so we use AddUObject.
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}
}

bool UMenu::Initialize() // This function Initializes immediately after the game is built.
{
	if (!Super::Initialize()) {
		return false;
	}

	if (Host_Button) {
		Host_Button->OnClicked.AddDynamic(this, &UMenu::HostButtonClicked);
	}

	if (Join_Button) {
		Join_Button->OnClicked.AddDynamic(this, &UMenu::JoinButtonClicked);
	}

	return true;
}

void UMenu::NativeDestruct() // This function is automatically called when the Widget is about to be destroyed.
{
	MenuTearDown(); // Removing Widget from Viewport and resetting the input and cursor mode. 

	Super::NativeDestruct(); // This way, when we change levels, we can no longer see the mouse cursor and add input to move the character. 
	// This calls the parent class's Native Destruct Method.
	// This ensures that any destruction logic defined in the base class is also executed. Failing to call Super::NativeDestruct() 
	// could lead to memory leaks or other issues, as the base class's cleanup code wouldn't be executed.
}

// Callback for our Custom OnCreateSession Delegate
void UMenu::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Yellow,
				FString(TEXT("Session Created Successfully"))
			);
		}

		// After Creating a Session is successful, we'll fly to the lobby.
		UWorld* World = GetWorld();
		if (World) {
			World->ServerTravel(PathToLobby); // Sends the Host to this lobby level. 
		}
	}
	else {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Failed to create Session"))
			);
		}
		Host_Button->SetIsEnabled(true);
	}
} 

// Callback for our Custom OnFindSession Delegate
void UMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (MultiplayerSessionsSubsystem == nullptr || !bWasSuccessful || SessionResults.Num() <= 0) {
		Find_Button->SetIsEnabled(true);
		//Join_Button->SetIsEnabled(true);
		return;
	}

	CurrentSessionLength = SessionResults.Num();
	UE_LOG(LogTemp, Warning, TEXT("SessionResults: %d"), CurrentSessionLength);

	for (auto Result : SessionResults) {
		UE_LOG(LogTemp, Warning, TEXT("Session found: %s"), *Result.GetSessionIdStr());
	}

	/*
	if (bWasSuccessful || SessionResults.Num() > 0) {
		CurrentSessionLength = SessionResults.Num();
		UE_LOG(LogTemp, Warning, TEXT("SessionResults: %d"), CurrentSessionLength);
	}
	else {
		Find_Button->SetIsEnabled(true);
	}
	*/

	/*
	// Loop through Session Results
	for (auto Result : SessionResults) {
		UE_LOG(LogTemp, Warning, TEXT("SessionResults: %d"), CurrentSessionLength);
		// We'll declare a FString Local Variable.
		FString SettingsValue;

		// We'll retrive the results, then access on the session, and then we'll have the session settings, and then we'll call the get function specifying a key.
		// The Key is called "MatchType", then we'll pass an FString and then the get function will fill in that FString with the value for this session.
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue); // Settings Value will be filled in with the value corresponding to this key "MatchType".

		// We'll check to see if it's the correct MatchType that matches our local variable MatchType.
		if (SettingsValue == MatchType) {
			// Then we'll call our JoinSession() function, passing in our FOnlineSessionSearchResult called "Result". Then if we find it, return.
			MultiplayerSessionsSubsystem->JoinSession(Result);
			return;
		}
	}

	if (!bWasSuccessful || SessionResults.Num() <= 0) {
		Join_Button->SetIsEnabled(true);
	}
	*/
}
// Callback for our Custom OnJoinSession Delegate
void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	// We just want to join the session, which means we'll need to get the correct Address and travel to it using client travel. 
	// We need to access the Online Session Interface and use the GetResolvedConnectString() function to retrieve the correct address.
	// We can access the Online Session Interface by itself, but we should not have our Custom Subsystem rely on Menu to retrieve the OnineSessionInterface.
	
	// This is how we obtain the Subsystem.
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if (Subsystem) {
		// This is how we get our Session Interface.
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();

		if (SessionInterface.IsValid()) { 
			FString Address; // This is Address Variable we made of type Fstring.
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address); // This returns the platform specific connection information (Address) for joining the match.

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController(); // We need to get the Player Controller from the Game Instance.
			if (PlayerController) {
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute); // Then we can use the Player Controller to access the ClientTravel function.
																					   // This allows us to travel to the address that we got from the GetResolvedConnectString.
			}
		}
	}
	if (Result != EOnJoinSessionCompleteResult::Success) {
		Join_Button->SetIsEnabled(true);
	}
}

// Callback for our Custom OnDestroySession Delegate
void UMenu::OnDestroySession(bool bWasSuccessful)
{
}

// Callback for our Custom OnStartSession Delegate
void UMenu::OnStartSession(bool bWasSucessful)
{
}

void UMenu::HostButtonClicked()
{
	Host_Button->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem) { // This checks if the Multiplayer Subsession is there.	
		MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType, SessionName);
	}
}

void UMenu::JoinButtonClicked()
{
	
	Join_Button->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem) {
		MultiplayerSessionsSubsystem->FindSessions(10000); // We'll access our Custom Subsystem to access our FindSession Function.
	}
	
}

void UMenu::FindButtonClicked()
{
	Find_Button->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem) {
		MultiplayerSessionsSubsystem->FindSessions(10000); // We'll access our Custom Subsystem to access our FindSession Function.
	}
}

void UMenu::GetHostInformation(int32 NumberOfPublicConnections, FString CurrentMatchType, FName TempSessionName)
{
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = CurrentMatchType;
	SessionName = TempSessionName;
}

void UMenu::MenuTearDown() 
{
	RemoveFromParent(); // To Remove Widget

	UWorld* World = GetWorld(); // Get Player Controller from the world
	if (World) {
		APlayerController* PlayerController = World->GetFirstPlayerController(); //This is how we get our first player controller. 
		if (PlayerController) {
			FInputModeGameOnly InputModeData; // Defining an InputModeGameOnly so we can focus on in the game. 
			PlayerController->SetInputMode(InputModeData); // Then we can add input and move our character.
			PlayerController->SetShowMouseCursor(false); // Removing the cursor from screen during game time. 
		}
	}
}

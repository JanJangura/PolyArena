// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Menu.h"

void USessionEntryWidget::Setup(const FOnlineSessionSearchResult& Result)
{
	SessionResult = Result;
	FString SessionName = Result.Session.OwningUserName;
	SessionNameText->SetText(FText::FromString(SessionName));

	JoinSessionButton->OnClicked.AddDynamic(this, &USessionEntryWidget::OnJoinSessionButtonClicked);
}

void USessionEntryWidget::OnJoinSessionButtonClicked()
{
	if (MenuRef && MenuRef->MultiplayerSessionsSubsystem && JoinSessionButton) {;
		MenuRef->MultiplayerSessionsSubsystem->JoinSession(MenuRef->CurrentSessionResults[SessionIndexRef]);
	}
	else {
		JoinSessionButton->SetIsEnabled(true);
	}
}

void USessionEntryWidget::SessionSetup(FString GetSessionID, FString GetSessionUser, FString GetMaxPlayers, int32 SessionIndex, FString SelectedMapType, UMenu* Menu)
{
	MenuRef = Menu;
	SessionIndexRef = SessionIndex;
	GetSessionName = GetSessionUser;

	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	this->SetIsFocusable(true);

	if (SessionNameText && JoinSessionButton && SessionMatchType && SessionPlayers && SessionMapType) {
		UE_LOG(LogTemp, Warning, TEXT("WORKING"));
		SessionNameText->SetText(FText::FromString(GetSessionUser));
		SessionMatchType->SetText(FText::FromString(GetSessionID));
		SessionPlayers->SetText(FText::FromString(GetMaxPlayers));
		SessionMapType->SetText(FText::FromString(SelectedMapType));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("NOT WORKING"));
	}
}

bool USessionEntryWidget::Initialize()
{
	if(!Super::Initialize()) return false;

	if (JoinSessionButton) {
		JoinSessionButton->OnClicked.AddDynamic(this, &USessionEntryWidget::OnJoinSessionButtonClicked);
	}

	return true;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USessionEntryWidget::Setup(const FOnlineSessionSearchResult& Result)
{
	SessionResult = Result;
	FString SessionName = Result.Session.OwningUserName;
	SessionNameText->SetText(FText::FromString(SessionName));

	JoinSessionButton->OnClicked.AddDynamic(this, &USessionEntryWidget::OnJoinSessionButtonClicked);
}

void USessionEntryWidget::OnJoinSessionButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Join Button Working"));
}

void USessionEntryWidget::TestingSetup(FString GetSessionID, FString GetSessionUser)
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	this->SetIsFocusable(true);

	if (SessionNameText && JoinSessionButton) {
		UE_LOG(LogTemp, Warning, TEXT("WORKING"));
		JoinSessionButton->OnClicked.AddDynamic(this, &USessionEntryWidget::OnJoinSessionButtonClicked);
		SessionNameText->SetText(FText::FromString(GetSessionUser));
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

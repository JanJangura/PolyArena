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

}

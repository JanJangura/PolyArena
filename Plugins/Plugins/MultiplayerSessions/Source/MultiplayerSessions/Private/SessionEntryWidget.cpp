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

void USessionEntryWidget::TestingSetup(int32 PlaceHolder)
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	this->SetIsFocusable(true);

	if (SessionNameText && JoinSessionButton) {
		UE_LOG(LogTemp, Warning, TEXT("WORKING"));
		SessionNameText->SetText(FText::FromString(FString::FromInt(PlaceHolder)));
		JoinSessionButton->OnClicked.AddDynamic(this, &USessionEntryWidget::OnJoinSessionButtonClicked);
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

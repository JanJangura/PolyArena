// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyBlasterHUD.h"
#include "BlasterGame/HUD/LaunchGameButton.h"

/*
void ALobbyBlasterHUD::BeginPlay()
{
	Super::BeginPlay();
	if (LaunchGameButton) {
		LaunchGameButton = CreateWidget<ULaunchGameButton>(GetOwningPlayerController(), LaunchGameButtonClass);

		if (LaunchGameButton)
		{
			LaunchGameButton->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create LaunchGameButton widget."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LaunchGameButtonClass is null."));
	}


}

void ALobbyBlasterHUD::HideLaunchGameButton()
{
	LaunchGameButton->SetVisibility(ESlateVisibility::Hidden);

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(false);
	PlayerController->bShowMouseCursor = false;

	UE_LOG(LogTemp, Warning, TEXT("Hidden"));
}

void ALobbyBlasterHUD::ShowLaunchGameButton()
{
	LaunchGameButton->SetVisibility(ESlateVisibility::Visible);

	FInputModeGameAndUI InputModeData;
	InputModeData.SetWidgetToFocus(LaunchGameButton->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(true);



	UE_LOG(LogTemp, Warning, TEXT("Visible"));
}

void ALobbyBlasterHUD::ToggleLaunchGameButton()
{
	if (PauseUICreated && PlayerController) {

		if (LaunchGameButton && LaunchGameButton->IsVisible()) {
			HideLaunchGameButton();
		}
		else {
			ShowLaunchGameButton();
		}
	}
}*/
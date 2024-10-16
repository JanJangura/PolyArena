// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyBlasterHUD.h"
#include "BlasterGame/HUD/LaunchGameButton.h"

ALobbyBlasterHUD::ALobbyBlasterHUD() {
	static ConstructorHelpers::FClassFinder<ULaunchGameButton> WidgetClass(TEXT("/Game/BP_Shooter_Character/Blueprints/HUD/WBP_LaunchGameButton"));
	if (WidgetClass.Succeeded())
	{
		LaunchGameButtonClass = WidgetClass.Class;
	}
}

void ALobbyBlasterHUD::BeginPlay()
{
	Super::BeginPlay();

	AddPauseUI();
}

void ALobbyBlasterHUD::AddPauseUI()
{
	if (PlayerController) {
		UE_LOG(LogTemp, Warning, TEXT("rEACHED"));
		if (LaunchGameButtonClass) {
			LaunchGameButton = CreateWidget<ULaunchGameButton>(PlayerController, LaunchGameButtonClass);
			LaunchGameButton->AddToViewport();
			LaunchGameButton->SetVisibility(ESlateVisibility::Hidden);
			UE_LOG(LogTemp, Warning, TEXT("rEACHED"));
		}
		PauseUICreated = true;
	}
}


void ALobbyBlasterHUD::HideLaunchGameButton()
{
	LaunchGameButton->SetVisibility(ESlateVisibility::Hidden);

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(false);
	PlayerController->bShowMouseCursor = false;
}

void ALobbyBlasterHUD::ShowLaunchGameButton()
{
	LaunchGameButton->SetVisibility(ESlateVisibility::Visible);

	FInputModeGameAndUI InputModeData;
	InputModeData.SetWidgetToFocus(LaunchGameButton->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(true);
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
}
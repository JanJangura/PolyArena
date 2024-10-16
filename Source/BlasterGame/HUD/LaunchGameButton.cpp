// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchGameButton.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "BlasterGame/GameMode/LobbyGameMode.h"
#include "BlasterGame/HUD/LobbyBlasterHUD.h"
#include "BlasterGame/PlayerController/BlasterPlayerController.h"


void ULaunchGameButton::NativeConstruct()
{
	Super::NativeConstruct();
	if (LaunchGameButton) {
		LaunchGameButton->OnClicked.AddDynamic(this, &ULaunchGameButton::LaunchButtonClicked);
	}
	UE_LOG(LogTemp, Warning, TEXT("Called 4"));
}

void ULaunchGameButton::LaunchButtonClicked()
{
	if (LaunchGameButton) {
		LaunchGameButton->SetIsEnabled(false);
	}
	
	AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);

	if (GameModeBase) {
		ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GameModeBase);
		
		if (LobbyGameMode) {
			LobbyTearDown();
			LobbyGameMode->ServerTravelToGame();
		}
	}
}

void ULaunchGameButton::LobbyTearDown()
{
	RemoveFromParent(); // To Remove Widget

	APlayerController* PlayerController = GetOwningPlayer(); //This is how we get our first player controller. 

	if (PlayerController) {

		//ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(PlayerController);
		
		class ALobbyBlasterHUD* LobbyBlasterHUD = Cast<ALobbyBlasterHUD>(PlayerController->GetHUD());

		if (LobbyBlasterHUD) {
			LobbyBlasterHUD->HideLaunchGameButton();
		}
	}
}

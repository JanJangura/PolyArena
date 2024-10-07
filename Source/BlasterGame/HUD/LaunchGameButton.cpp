// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchGameButton.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "BlasterGame/GameMode/LobbyGameMode.h"

bool ULaunchGameButton::Initialize()
{
	if (!Super::Initialize()) {
		return false;
	}

	if (LaunchGameButton) {
		UE_LOG(LogTemp, Warning, TEXT("LaunchGameButtonClicked"));
		LaunchGameButton->OnClicked.AddDynamic(this, &ULaunchGameButton::LaunchButtonClicked);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("LaunchGameButtonClicked"));
	}

	return true;
}

void ULaunchGameButton::LaunchButtonClicked()
{
	LaunchGameButton->SetIsEnabled(false);

	AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);

	UE_LOG(LogTemp, Warning, TEXT("LaunchGameButtonClicked"));

	if (GameModeBase) {
		ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GameModeBase);
		
		if (LobbyGameMode) {
			LobbyGameMode->LaunchGameButtonClicked = true;
			UE_LOG(LogTemp, Warning, TEXT("LaunchGameButtonClicked"));
		}
	}
}

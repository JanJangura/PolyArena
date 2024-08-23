// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Character.h"

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText) {
		// This function will set the text for this text block.
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	// We can get the local role of the InPawn. A player's role is an enum value, and that enum is ENetRole. 
	ENetRole LocalRole = InPawn->GetLocalRole();	// GetLocalRole() is a function that we can call from a pawn to see what its local role is.
	
	// We can then use the information from GetLocalRole() and set our display text function to show that role.
	FString Role;
	// We can set the value of Role Depending on our LocalRole.
	switch (LocalRole) {
	case ENetRole::ROLE_Authority:
		Role = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		Role = FString("AutonomousProxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		Role = FString("SimulatedProxy");
		break;
	case ENetRole::ROLE_None:
		Role = FString("None");
		break;
	}

	// This is how we'll format our local variable and pass it into SetDisplayText.
	FString LocalRoleString = FString::Printf(TEXT("Local Role: %s"), *Role);
	SetDisplayText(LocalRoleString);
}

void UOverheadWidget::GetPlayerName(ACharacter* Character)
{
	APlayerController* PlayerController;

	if (Character) {
		PlayerController = Cast<APlayerController>(Character->GetController());

		APlayerState* PlayerState;

		if (PlayerController) {
			PlayerState = PlayerController->GetPlayerState<APlayerState>();

			if (PlayerState) {
				FString PlayerName = PlayerState->GetPlayerName();
				SetDisplayText(PlayerName);
			}
		}
	}
}

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();

	Super::NativeDestruct();
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterPlayerController.h"
#include "BlasterGame/HUD/BlasterHUD.h"
#include "BlasterGame/HUD/CharacterOverlay.h"
#include "BlasterGame/HUD/LaunchGameButton.h"
#include "Components/TextBlock.h"
#include "BlasterGame/Character/BlasterCharacter.h"



void ABlasterPlayerController::SetHUDWeaponAmmo(int32 Ammo)
{
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay && 
		BlasterHUD->CharacterOverlay->WeaponAmmoAmount;

	if (bHUDValid) {
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		BlasterHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(AmmoText));
	}
}

void ABlasterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ABlasterCharacter>(GetCharacter());
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
}

void ABlasterPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ABlasterPlayerController::TogglePauseMenu()
{

}

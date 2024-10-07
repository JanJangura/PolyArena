// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterPlayerController.h"
#include "BlasterGame/HUD/BlasterHUD.h"
#include "BlasterGame/HUD/CharacterOverlay.h"
#include "BlasterGame/HUD/LaunchGameButton.h"
#include "Components/TextBlock.h"
#include "BlasterGame/Character/BlasterCharacter.h"
#include "BlasterGame/HUD/LobbyBlasterHUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/HUD.h"

ABlasterPlayerController::ABlasterPlayerController()
{
	static ConstructorHelpers::FClassFinder<ABlasterHUD> HUDClassFinder(TEXT("/Game/BP_Shooter_Character/Blueprints/HUD/BP_BlasterHUD"));
	SpecialHUDClass = HUDClassFinder.Class;

	// Ensure that SpecialHUDClass is valid
	if (!SpecialHUDClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpecialHUDClass could not be found!"));
	}
}

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

void ABlasterPlayerController::SetNewHUD()
{
	UE_LOG(LogTemp, Warning, TEXT("SetNewHUD;"));

	if (!SpecialHUDClass) { UE_LOG(LogTemp, Warning, TEXT("SpecialHUDClass;")); return; }

	if (CurrentHUD) {
		CurrentHUD->Destroy();
		CurrentHUD = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("CurrentHUD->Destroy();"));
	}

	CurrentHUD = GetWorld()->SpawnActor<ABlasterHUD>(SpecialHUDClass);
	UE_LOG(LogTemp, Warning, TEXT("CurrentHUD"));

	if (CurrentHUD)
	{
		BlasterHUD = Cast<ABlasterHUD>(CurrentHUD);
	
		UE_LOG(LogTemp, Warning, TEXT("New HUD spawned and assigned successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn new HUD!"));
	}
}

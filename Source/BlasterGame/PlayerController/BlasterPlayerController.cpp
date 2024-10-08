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
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

ABlasterPlayerController::ABlasterPlayerController()
{
	/*
	static ConstructorHelpers::FClassFinder<ABlasterHUD> HUDClassFinder(TEXT("/Game/BP_Shooter_Character/Blueprints/HUD/BP_BlasterHUD"));
	SpecialHUDClass = HUDClassFinder.Class;

	// Ensure that SpecialHUDClass is valid
	if (!SpecialHUDClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpecialHUDClass could not be found!"));
	}
	*/
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
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetCharacter()) : Character;
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
}

void ABlasterPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ABlasterPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD && 
		BlasterHUD->CharacterOverlay && 
		BlasterHUD->CharacterOverlay->HealthBar && 
		BlasterHUD->CharacterOverlay->HealthText;

	if (bHUDValid) {
		const float HealthPercent = Health / MaxHealth;
		BlasterHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		BlasterHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}

}

/*
void ABlasterPlayerController::SetNewHUD()
{
	UE_LOG(LogTemp, Warning, TEXT("SetNewHUD;"));

	if (!SpecialHUDClass) { UE_LOG(LogTemp, Warning, TEXT("SpecialHUDClass;")); return; }

	
	if (GEngine) {
		GEngine->GameViewport->GetGameInstance()->GetFirstLocalPlayerController()->ClientSetHUD(SpecialHUDClass);
	}
}
*/
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
#include "Net/UnrealNetwork.h"
#include "BlasterGame/GameMode/BlasterGameMode.h"
#include "BlasterGame/HUD/Announcement.h"
#include "BlasterGame/BlasterComponents/CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BlasterGame/GameState/BlasterGameState.h"
#include "BlasterGame/PlayerState/BlasterPlayerState.h"
#include "BlasterGame/HUD/ReturnToMainMenu.h"
#include "BlasterGame/HUD/PlayerList.h"
#include "BlasterGame/HUD/PlayerInfo.h"
#include "Components/ScrollBox.h"
#include "Components/Widget.h"

void ABlasterPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterPlayerController, MatchState);
}

void ABlasterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent == nullptr) return; // This variable is a UInputComponent variable inherited by the PlayerController. 

	// This is how we set up our binding Action.
	InputComponent->BindAction("Quit", IE_Pressed, this, &ABlasterPlayerController::ShowReturnToMainMenu);
	InputComponent->BindAction("PlayerList", IE_Pressed, this, &ABlasterPlayerController::ShowPlayerList);
	InputComponent->BindAction("PlayerList", IE_Released, this, &ABlasterPlayerController::ShowPlayerList);
}

void ABlasterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetCharacter()) : Character;
	CastBlasterHUD();

	ServerCheckMatchState();

}

void ABlasterPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDTime();
	
	CheckTimeSynch(DeltaTime);
	PollInit();
}

void ABlasterPlayerController::CastBlasterHUD()
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
}

void ABlasterPlayerController::PollInit()
{
	if (CharacterOverlay == nullptr && BlasterHUD && BlasterHUD->CharacterOverlay) {
		
		CharacterOverlay = BlasterHUD->CharacterOverlay;

		if (CharacterOverlay) {

			SetHUDHealth(HUDHealth, HUDMaxHealth);
			SetHUDScore(HUDScore);
			SetHUDDefeats(HUDDefeats);
			//if (bInitializeCarriedAmmo) SetHUDCarriedAmmo(HUDCarriedAmmo);
			if (bInitializeWeaponAmmo) SetHUDWeaponAmmo(HUDWeaponAmmo);
			if (bInitializeWeaponIcon) SetWeaponIcon(PrimaryWeaponType);
		}
	}
}
// Show the Return To Main Menu Widget
void ABlasterPlayerController::ShowReturnToMainMenu()
{
	if (ReturnToMainMenuWidget == nullptr) return;
	
	ReturnToMainMenu = ReturnToMainMenu == nullptr ? CreateWidget<UReturnToMainMenu>(this, ReturnToMainMenuWidget) : ReturnToMainMenu;

	if (ReturnToMainMenu && !bPlayerListTab) {
		bReturnToMainMenuOpen = !bReturnToMainMenuOpen;
		if (bReturnToMainMenuOpen) {
			ReturnToMainMenu->MenuSetup();
		}
		else {
			ReturnToMainMenu->MenuTearDown();
		}
	}
}

void ABlasterPlayerController::ShowPlayerList()
{
	if (PlayerList && !bReturnToMainMenuOpen && bInProgressState) {
		bPlayerListTab = !bPlayerListTab;
		if (bPlayerListTab) {
			PlayerList->PlayerListSetup();
		}
		else {
			PlayerList->PlayerListTearDown();
		}
	}
}

// Server RPC
void ABlasterPlayerController::ServerCheckMatchState_Implementation()
{
	ABlasterGameMode* GameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));

	if (GameMode) {
		WarmupTime = GameMode->WarmupTime;
		MatchTime = GameMode->MatchTime;
		CooldownTime = GameMode->CooldownTime;
		LevelStartingTime = GameMode->LevelStartingTime;
		MatchState = GameMode->GetMatchState();

		ClientJoinMidGame(MatchState, WarmupTime, MatchTime, CooldownTime, LevelStartingTime);
	}
}

// Client RPC
void ABlasterPlayerController::ClientJoinMidGame_Implementation(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime)
{
	WarmupTime = Warmup;
	MatchTime = Match;
	CooldownTime = Cooldown;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;

	OnMatchStateSet(MatchState); // In case the stuff above happens after the variable replication for Match State has already happened.

	if (BlasterHUD && MatchState == MatchState::WaitingToStart) {
		BlasterHUD->AddAnouncement();
	}
}

void ABlasterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CastBlasterHUD();

	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(InPawn);

	if (BlasterCharacter) {
		SetHUDHealth(BlasterCharacter->GetHealth(), BlasterCharacter->GetMaxHealth());
		BlasterCharacter->UpdateHUDAmmo();
		BlasterCharacter->UpdateWeaponIcon();
	}
}

ABlasterPlayerController::ABlasterPlayerController()
{
	
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
	else {
		bInitializeWeaponAmmo = true;
		HUDWeaponAmmo = Ammo;
	}
}

void ABlasterPlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->CarriedAmmoAmount;

	if (!bHUDValid) {
		return;
	}

	if (bHUDValid) {
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		//FString AmmoText = FString::Printf(TEXT("INFINITE"));
		BlasterHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
	}
	else {
		bInitializeCarriedAmmo = true;
		HUDCarriedAmmo = Ammo;
	}
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
	else {
		bInitializeCharacterOverlay = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
	}
}

void ABlasterPlayerController::SetWeaponIcon(EWeaponType WeaponType)
{
	bool bHUDValid = BlasterHUD && 
		BlasterHUD->CharacterOverlay && 
		BlasterHUD->CharacterOverlay->WeaponSelectionText;

	if (bHUDValid)
	{
		FString WeaponText;
		switch (WeaponType)
		{
		case EWeaponType::EWT_AssaultRifle:
			WeaponText = "Assault Rifle";
			break;
		case EWeaponType::EWT_GrenadeLauncher:
			WeaponText = "GrenadeLauncher";
			break;
		case EWeaponType::EWT_SniperRifle:
			WeaponText = "SniperRifle";
			break;
		case EWeaponType::EWT_Pistol:
			WeaponText = "Pistol";
			break;
		case EWeaponType::EWT_SubMachineGun:
			WeaponText = "SubMachineGune";
			break;
		case EWeaponType::EWT_RocketLauncher:
			WeaponText = "RocketLauncher";
			break;
		case EWeaponType::EWT_Shotgun:
			WeaponText = "Shotgun";
			break;
		case EWeaponType::EWT_None:
		default:
			WeaponText = "None";
			break;
		}

		BlasterHUD->CharacterOverlay->WeaponSelectionText->SetText(FText::FromString(WeaponText));
	}
	else {
		bInitializeWeaponIcon = true;
		if (BlasterHUD) {
			if (BlasterHUD->HasAuthority()) {
				PrimaryWeaponType = WeaponType;
				UE_LOG(LogTemp, Warning, TEXT("SERVER: bInitializeWeaponIcon is %s"), bInitializeWeaponIcon ? TEXT("true") : TEXT("false"));
			}
			else {
				PrimaryWeaponType = WeaponType;
				UE_LOG(LogTemp, Warning, TEXT("CLIENT: bInitializeWeaponIcon is %s"), bInitializeWeaponIcon ? TEXT("true") : TEXT("false"));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("CLIENT: BlasterHUD is %s"), BlasterHUD ? TEXT("true") : TEXT("false"));
		}
	}
}

void ABlasterPlayerController::BroadCastElim(APlayerState* Attacker, APlayerState* Victim)
{
	ClientElimAnnouncement(Attacker, Victim);
}

// Client RPC
void ABlasterPlayerController::ClientElimAnnouncement_Implementation(APlayerState* Attacker, APlayerState* Victim)
{
	// Compare Attacker and Victim to Self.
	APlayerState* Self = GetPlayerState<APlayerState>();
	if (Attacker && Victim && Self) {

		BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
		if (BlasterHUD) {
			if (Attacker == Self && Victim != Self) {
				BlasterHUD->AddElimAnnouncement("You", Victim->GetPlayerName());
				return;
			}
			else if (Attacker != Self && Victim == Self) {
				BlasterHUD->AddElimAnnouncement(Attacker->GetPlayerName(), "You");
				return;
			}
			else if (Attacker == Self && Victim == Self) {
				BlasterHUD->AddElimAnnouncement("You", "Yourself");
				return;
			}
			else if (Attacker == Victim && Attacker != Self) {
				BlasterHUD->AddElimAnnouncement(Attacker->GetPlayerName(), "Themselves");
				return;
			}
			BlasterHUD->AddElimAnnouncement(Attacker->GetPlayerName(), Victim->GetPlayerName());
		}
	}
}

void ABlasterPlayerController::SetHUDScore(float Score)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->ScoreAmount;

	if (bHUDValid) {
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		BlasterHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromStringView(ScoreText));
	}
	else {
		bInitializeCharacterOverlay = true;
		HUDScore = Score;
	}
}

void ABlasterPlayerController::SetHUDDefeats(int32 Defeats)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->DefeatsAmount;

	if (bHUDValid) {
		FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
		BlasterHUD->CharacterOverlay->DefeatsAmount->SetText(FText::FromStringView(DefeatsText));
	}
	else {
		bInitializeCharacterOverlay = true;
		HUDDefeats = Defeats;
	}
}

// This is called at the beginning of the game. 
void ABlasterPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (IsLocalController()) {
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void ABlasterPlayerController::SetHUDTime()
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	if (!BlasterHUD) return;

	float TimeLeft = 0.f;
	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::Cooldown) TimeLeft = CooldownTime + WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);// This is how we would get the time within the world in seconds and it's rounded to the next integer.

	if (HasAuthority()) {

		BlasterGameMode = BlasterGameMode == nullptr ? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
		if (BlasterGameMode) {
			TimeLeft = BlasterGameMode->GetCountdownTime() + LevelStartingTime;
			SecondsLeft = FMath::CeilToInt(TimeLeft);
		}
	}

	if (CountdownInt != SecondsLeft) { // This allows us to change the HUD every second and not every frame.
		if (MatchState == MatchState::WaitingToStart) {
			SetHUDAnnouncementCountdown(TimeLeft);
		}
		else if (MatchState == MatchState::InProgress) {
			SetHUDMatchCountdown(TimeLeft);
		}
		else if (MatchState == MatchState::Cooldown) {
			SetHUDAnnouncementCountdown(TimeLeft);
		}
	}
	CountdownInt = SecondsLeft;
}

void ABlasterPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->MatchCountdownText;

	bool bHUDWarmupTimer = BlasterHUD && BlasterHUD->Announcement;

	if (bHUDValid) {
		if (CountdownTime <= 0.f) {
			if(bHUDWarmupTimer) BlasterHUD->Announcement->WarmupTime->SetText(FText());
			return;
		}

		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		BlasterHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromStringView(CountdownText));
	}
}

void ABlasterPlayerController::SetHUDAnnouncementCountdown(float CountdownTime)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->Announcement &&
		BlasterHUD->Announcement->WarmupTime;

	if (bHUDValid) {
		if (CountdownTime < 0.f) {
			BlasterHUD->Announcement->WarmupTime->SetText(FText());
			return;
		}

		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		BlasterHUD->Announcement->WarmupTime->SetText(FText::FromStringView(CountdownText));
	}
}

float ABlasterPlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	else return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void ABlasterPlayerController::CheckTimeSynch(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency) {
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

void ABlasterPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void ABlasterPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeOfServerReceivedCientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeOfServerReceivedCientRequest + (0.5f * RoundTripTime);
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

// This happens on the Server.
void ABlasterPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;

	if (MatchState == MatchState::InProgress) {
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown) {
		HandleCooldown();
	}
}

void ABlasterPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress) {
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown) {
		HandleCooldown();
	}
}

void ABlasterPlayerController::HandleMatchHasStarted()
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if (BlasterHUD) {
		if(BlasterHUD->CharacterOverlay == nullptr) BlasterHUD->AddCharacterOverlay();
		if (BlasterHUD->Announcement) {
			BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ABlasterPlayerController::HandleCooldown()
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if (BlasterHUD) {
		BlasterHUD->CharacterOverlay->RemoveFromParent();

		bool BHUDValid = BlasterHUD->Announcement 
			&& BlasterHUD->Announcement->AnnouncementText 
			&& BlasterHUD->Announcement->InfoText;

		if (BHUDValid) {

			BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Visible);

			FString AnnouncementText("Leaving to Main Menu in: ");
			BlasterHUD->Announcement->AnnouncementText->SetText(FText::FromString(AnnouncementText));

			// This will always return us the current Game State as long as we pass in a World Context Object, so it knows which world to search in. 
			ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
			ABlasterPlayerState* BlasterPlayerState = GetPlayerState <ABlasterPlayerState>();

			if (BlasterGameState && BlasterPlayerState) {

				TArray<ABlasterPlayerState*> TopPlayers = BlasterGameState->TopScoringPlayers;
				FString InfoTextString;
					
				if (TopPlayers.Num() == 0) {
					InfoTextString = FString("There Are No Winners!");
				}
				else if (TopPlayers.Num() == 1 && TopPlayers[0] == BlasterPlayerState) { // This is if there is only one player in the Game, this is a personal message to that Player.
					InfoTextString = FString("You Are The Winner!");
				}
				else if (TopPlayers.Num() == 1) { // This is if there is only one Top Player.
					InfoTextString = FString::Printf(TEXT("Winner:\n%s"), *TopPlayers[0]->GetPlayerName());
				}
				else if (TopPlayers.Num() > 1) { // This is if there are more than one Top Player. 
					InfoTextString = FString("Players Tied For The Win:\n");
					for (auto TiedPlayer : TopPlayers) { // auto TiedPlayer means that for each loop iteration, tied players will be one of the players within the PlayerStateArray.
						InfoTextString.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName())); // This will Append more Text onto the FString at the end.
					}
				}

				BlasterHUD->Announcement->InfoText->SetText(FText::FromString(InfoTextString));
			}

			
		}
	}

	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(GetPawn());
	if (BlasterCharacter && BlasterCharacter->GetCombat()) {
		BlasterCharacter->bDisableGameplay = true;
		BlasterCharacter->GetCombat()->FireButtonPressed(false);
	}
}

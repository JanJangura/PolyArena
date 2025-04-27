// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "BlasterGame/Character/BlasterCharacter.h"
#include "BlasterGame/BlasterComponents/CombatComponent.h"
#include "BlasterGame/Weapon/Weapon.h"
#include "BlasterGame/GameMode/BlasterGameMode.h"
#include "Kismet/GameplayStatics.h" 

void AAmmoPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter) {

		UCombatComponent* Combat = BlasterCharacter->GetCombat();
		if (Combat) {
			UE_LOG(LogTemp, Warning, TEXT("Equipped Weapon is VALID!!"));
			Combat->PickupAmmo(this, WeaponType, AmmoAmount, AmmoLocation);
			//Destroy();
		}
	}
}

void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();
	ThisActorLocation = GetActorLocation();
}

void AAmmoPickup::StartRespawnTimer()
{
	// Start a timer for respawn
	GetWorldTimerManager().SetTimer(
		RespawnTimerHandle,
		this,
		&AAmmoPickup::FinishRespawn,
		AmmoRespawnCooldown,
		false
	);
}

void AAmmoPickup::FinishRespawn()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "BlasterGame/Character/BlasterCharacter.h"
#include "BlasterGame/BlasterComponents/CombatComponent.h"
#include "BlasterGame/Weapon/Weapon.h"

void AAmmoPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter) {

		UCombatComponent* Combat = BlasterCharacter->GetCombat();
		if (Combat && BlasterCharacter->GetEquippedWeapon()) {
			UE_LOG(LogTemp, Warning, TEXT("Equipped Weapon is VALID!!"));
			Combat->PickupAmmo(this, WeaponType, AmmoAmount);
			//Destroy();
		}
		if (Combat && Combat->GetSecondaryWeapon()) {
			UE_LOG(LogTemp, Warning, TEXT("Secondary Weapon is VALID!!"));
			Combat->PickupAmmo(this, WeaponType, AmmoAmount);
		}
	}
}

void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();
	ThisActorLocation = GetActorLocation();
}

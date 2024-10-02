// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "BlasterGame/Weapon/Weapon.h"
#include "BlasterGame/Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 450.f;
}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character) {
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	// On the client, this aiming animation will work first on that client before it's sent through an RPC reaching the server. Therefore, the aiming
	// mechanics on the client will execute first before reaching the server and this is fine.
	bAiming = bIsAiming;

	// We don't need to check if the character is on the server, because no matter what, it will be executed on the server even if were on the client.
	// So calling this function on the server means we're replicating it to all other clients. 
	ServerSetAiming(bIsAiming);	

	// If our Character is Aiming, set the Walk Speed to Aim Speed / else leave it at BaseWalkSpeed.
	if (Character) {
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

// For RPC, we have to add the "_Implementation" so Unreal can create the real definition for it.
void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	// This is the RPC that we sent to the server, so the server knows to replicate the aiming animation for all players holding (RMB). 
	bAiming = bIsAiming;
	if (Character) {
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

// RepNotifier for our Equipped Weapon Animation.
void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Character) {
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;
	}
}

void UCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;

	if (EquippedWeapon == nullptr) { return; }

	if (Character && bFireButtonPressed) {
		Character->PlayFireMontage(bAiming);	// Play the montage for the character.
		EquippedWeapon->Fire();	// Play the weapons animation.
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// We will always need this for Replication. We have to define on the list what we want to Replicate here.
void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Now when this value changes, it will replicate in all clients.
	DOREPLIFETIME(UCombatComponent, EquippedWeapon);	

	// Now when this value changes, it will replicate in all clients.
	DOREPLIFETIME(UCombatComponent, bAiming);	
}

// We're gonna replicate this
void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) {	// Checking for validation
		return;
	}

	EquippedWeapon = WeaponToEquip;	// Setting our Equipped Weapon instance to this "WeaponToEquip" class reference.
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);	// This sets our WeaponState to "Equipped".

	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));	// We'll define a HandSocket variable of type USkeletalMeshSocket.
																												// Then, we'll retrieve our RightHandSocket that made and defined.
	if (HandSocket) {	
		HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());	// In the HandSocket that we created to hold our weapon, we'll attach our "EquippedWeapon" reference.	
		// AttachActor requires the Actor that attaches to the socket and the skeletal mesh component associated with the socket.
	}

	// We need the weapon owner to be the character. Ownership is very important. We own the pawn that we're controlling, but an actor (like a weapon), doesn't have a defined
	// owner, but as soon as we equip it, we should set it's owner to the character that has equipped it.
	EquippedWeapon->SetOwner(Character);	// This SetOwner() function sets the owner of the EquippedWeapon to the actor that's passed in reference (Character).
	
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;
	// RPC can be called both ways, so we could make an RPC to be called from the server and executed on a client, but we can use variable replication instead.
}

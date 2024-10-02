// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "BlasterGame/Weapon/Weapon.h"
#include "BlasterGame/Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 450.f;
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
{									// Remember, Rep_Notifiers is only good for when we change the value of a variable.
	bFireButtonPressed = bPressed;	// We don't want to use REP_NOTIFIERS with this boolean is because we're going to use Automatic Weapons. This will become expensive and demanding.
									// We may need true to be played for a while because of the automatic fire. We'll need to use multicast RPC to do this.
									// The NetMulticast RPC called from the server runs on server and clients. Where as the Server RPC only runs on the server, even when called on 
									// the Client. When calling NetMulticast RPC from Client, it only runs on the Client (kinda worthless on client, more used on Server).
	if (bFireButtonPressed) {
		ServerFire();	// Calling the Server RPC ServerFire(), so this function is only called on the server and not on any other clients
						// and we don't need to include "_Implementation" to call it. Now we just need to do it on all Clients. We don't even need to check if this is on Server
						// or not because we already declared it as a Server RPC.
	}
}

// This where we're going to have our projectiles launch from the center of the screen.
void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;	// We need our viewport size, the dimensions of it. Essentially the screen of what players see. 
	if (GEngine && GEngine->GameViewport) {	// We can get our Viewport from GEngine, but first we need to make sure that it's available.
		GEngine->GameViewport->GetViewportSize(ViewportSize);	// This is how we get our Viewport Size from the function "GetViewportSize()".
	}

	// We can initalize the size of of our Vector, with the size of X and Y.
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f); // Now that we got our Viewport Size, we can get the X and Y Dimensions and manipulate to 
															// get the center of the screen. Yet this is only local space and it's only in 2D. We need to take this information
															// and transform it into 3D information.
	// This is how we'll turn our 2D screen space coordinate into 3D world-space point and direction.
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	
	// We'll need to get the player controller and we can do this by calling player zero. Even though it's a multiplayer game, each machine has a player 0 playing the game.
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0), // "this" satifies the WorldContextObject (what were referencing in the world), and then "0" is the player Index. 
		CrosshairLocation, // Now we'll need an FVector 2D screen position, the position that we'd like to project into world coordinates, which is our CrosshairLocation.
		CrosshairWorldPosition,	// World Position Vector
		CrosshairWorldDirection	// World Direction
	);	// After calling DeprojectScreenToWorld() function, our World Position and World Direction FVectors will automatically be filled in.
	// DeprojectScreenToWorld returns a boolean if this DeProjection is successful, so we can store this.
	
	if (bScreenToWorld) {	// If this is true, we've successfully got our 2D screen coordinates to 3D Projection.
		FVector Start = CrosshairWorldPosition; // The starting position for our line trace.
		FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;	// So End starts at the Start position (Center of our Screen), and project out forward 
														// in the crosshair world direction by one unit, but we can move it out forward more by multiplying by something large.
		// To perform the line trace.
		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,	// This will automatically be filled in by this LineTraceSingleByChannel. 
			Start,	// This is where the Line Trace start.
			End,	// This is where the Line Trace end.
			ECollisionChannel::ECC_Visibility	// We also need a Collision Channel, we'll use visibility as this is the most common. Rest of the stuff is optional.
		);	// Now our Line Trace will hit something and retain that information whenever it hits something. In the case that we Trace and it doesn't hit something,
		// we still need an endpoint so we can launch our projectile in that direction.
		if (!TraceHitResult.bBlockingHit) {
			TraceHitResult.ImpactPoint = End;	// This will at least allow us to have an impact point in our trace hit result.
			// ImpactPoint is an FVector, that is the world position of our Line Trace if we hit something.
			HitTarget = End;
		}
		else {
			HitTarget = TraceHitResult.ImpactPoint;
			// Draw A debug sphere here cause we know we got a blocking hit.
			DrawDebugSphere(
				GetWorld(),
				TraceHitResult.ImpactPoint,
				12.f,
				12,
				FColor::Red
				);
		}
	}
}

// *REMEMBER, RPC needs _Implementation added to the function.	
void UCombatComponent::ServerFire_Implementation()
{
	MulticastFire();	// We can call this NetMulticast RPC "MulticastFire()" function here, because we need to call it on the server in order to replicate the fire effect for
						// everyone, both clients and server.
}

// This is our NetMulticast RPC, and we'll need to call this from the Server RPC so all clients and server host can replicate the firing effects of our weapon.
void UCombatComponent::MulticastFire_Implementation()
{
	if (EquippedWeapon == nullptr) { return; }

	if (Character) {
		Character->PlayFireMontage(bAiming);	// Play the montage for the character.
		EquippedWeapon->Fire(HitTarget);	// Play the weapons animation.
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);
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

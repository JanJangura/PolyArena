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
#include "BlasterGame/PlayerController/BlasterPlayerController.h"
#include "BlasterGame/HUD/BlasterHUD.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "BlasterGame/HUD/LobbyBlasterHUD.h"

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

	// This only matters for the owning client, so we need Condition and clarify that it only applies to the Owner. "DOREPLIFETIME_CONDITION" and "COND_OwnerOnly" is used here.
	DOREPLIFETIME_CONDITION(UCombatComponent, CarriedAmmo, COND_OwnerOnly);

	// Secondary Weapon Replication
	DOREPLIFETIME(UCombatComponent, SecondaryWeapon);
}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character) {
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

		if (Character->GetFollowCamera()) {
			DefaultFOV = Character->GetFollowCamera()->FieldOfView; // "FieldOfView" is a variable for the camera's current Field of View. This should be our DefaultFOV
			CurrentFOV = DefaultFOV;
		}
		if (Character->HasAuthority()) {
			InitializeCarriedAmmo();
		}
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Character && Character->IsLocallyControlled()) {
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		SetHUDCrossHairs(DeltaTime);
		InterpFOV(DeltaTime);
		HitTarget = HitResult.ImpactPoint;
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

void UCombatComponent::FireButtonPressed(bool bPressed)
{									// Remember, Rep_Notifiers is only good for when we change the value of a variable.
	bFireButtonPressed = bPressed;	// We don't want to use REP_NOTIFIERS with this boolean is because we're going to use Automatic Weapons. This will become expensive and demanding.
									// We may need true to be played for a while because of the automatic fire. We'll need to use multicast RPC to do this.
									// The NetMulticast RPC called from the server runs on server and clients. Where as the Server RPC only runs on the server, even when called on 
									// the Client. When calling NetMulticast RPC from Client, it only runs on the Client (kinda worthless on client, more used on Server).
	if (bFireButtonPressed && EquippedWeapon) {
		Fire();
	}
}

void UCombatComponent::Fire()
{
	if (bCanFire) {
		bCanFire = false;

		GetCenterOfCameraTransform();

		ServerFire(HitTarget);	// Calling the Server RPC ServerFire(), so this function is only called on the server and not on any other clients
		// and we don't need to include "_Implementation" to call it. Now we just need to do it on all Clients. We don't even need to check if this is on Server
		// or not because we already declared it as a Server RPC. ".ImpactPoint" is a FVector_NetQuantize all along, already optimized for multiplayer.

		if (EquippedWeapon) {
			CrosshairShootingFactor += 1.f;
			CrosshairShootingFactor = FMath::Clamp(CrosshairShootingFactor, 0.f, 3.f);
			//UE_LOG(LogTemp, Error, TEXT("FireButtonPressed"));	
		}

		StartFireTimer();
	}
}

void UCombatComponent::StartFireTimer()
{
	if (EquippedWeapon == nullptr || Character == nullptr) { UE_LOG(LogTemp, Warning, TEXT("StartFireTimerReturnedNull!!"));  return; }

		Character->GetWorldTimerManager().SetTimer(
			FireTimer,
			this,
			&UCombatComponent::FireTimerFinished,
			EquippedWeapon->FireRate
		);
	
}

void UCombatComponent::FireTimerFinished()
{
	if (EquippedWeapon == nullptr) { return; }

	bCanFire = true;

	if (bFireButtonPressed && EquippedWeapon->bAutomatic) {
		Fire();
	}
}

void UCombatComponent::PauseButtonToggle()
{
	LobbyBlasterHUD = LobbyBlasterHUD == nullptr ? Cast<ALobbyBlasterHUD>(Controller->GetHUD()) : LobbyBlasterHUD;

	if (LobbyBlasterHUD) {
		LobbyBlasterHUD->ToggleLaunchGameButton();
	}
}

void UCombatComponent::OnRep_CarriedAmmo()
{
	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDCarriedAmmo(CarriedAmmo);
	}
}

void UCombatComponent::InitializeCarriedAmmo()
{
	CarriedAmmoMap.Emplace(EWeaponType::EWT_AssaultRifle, StartingARAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Pistol, StartingPistolAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_RocketLauncher, StartingRocketAmmo);
}

bool UCombatComponent::ShouldSwapWeapons()
{
	return (EquippedWeapon != nullptr && SecondaryWeapon != nullptr);
}

// *REMEMBER, RPC needs _Implementation added to the function.	
void UCombatComponent::ServerFire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	MulticastFire(TraceHitTarget);	// We can call this NetMulticast RPC "MulticastFire()" function here, because we need to call it on the server in order to replicate the fire effect for
						// everyone, both clients and server.
}

// This is our NetMulticast RPC, and we'll need to call this from the Server RPC so all clients and server host can replicate the firing effects of our weapon.
void UCombatComponent::MulticastFire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if (EquippedWeapon == nullptr) { return; }

	if (Character) {
		EquippedWeapon->CenterOfCameraTransform = GetCenterOfCameraTransform();
		Character->PlayFireMontage(bAiming);	// Play the montage for the character.
		EquippedWeapon->Fire(TraceHitTarget);	// Play the weapons animation.
	}
}

// This is on the Server.
void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) {	// Checking for validation
		return;
	}

	if (EquippedWeapon != nullptr && SecondaryWeapon == nullptr) {
		EquipSecondaryWeapon(WeaponToEquip);
	}
	else {
		EquipPrimaryWeapon(WeaponToEquip);
	}

	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;
	// RPC can be called both ways, so we could make an RPC to be called from the server and executed on a client, but we can use variable replication instead.
}

void UCombatComponent::SwapWeapons()
{
	AWeapon* TempWeapon = EquippedWeapon;
	EquippedWeapon = SecondaryWeapon;
	PrimaryWeapon = EquippedWeapon;
	SecondaryWeapon = TempWeapon;

	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));	// We'll define a HandSocket variable of type USkeletalMeshSocket.
	// Then, we'll retrieve our RightHandSocket that made and defined.
	if (HandSocket) {
		HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());	// In the HandSocket that we created to hold our weapon, we'll attach our "EquippedWeapon" reference.	
		// AttachActor requires the Actor that attaches to the socket and the skeletal mesh component associated with the socket.
	}

	EquippedWeapon->SetHUDAmmo();

	// Carried Ammo is set here.
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType())) {
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}

	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDCarriedAmmo(CarriedAmmo);
	}

	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
	AttachActorToBackpack(SecondaryWeapon);
}

void UCombatComponent::EquipPrimaryWeapon(AWeapon* WeaponToEquip)
{
	if (EquippedWeapon) {
		EquippedWeapon->Dropped();
	}

	EquippedWeapon = WeaponToEquip;	// Setting our Equipped Weapon instance to this "WeaponToEquip" class reference.
	PrimaryWeapon = EquippedWeapon;
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
	EquippedWeapon->SetHUDAmmo();

	// Carried Ammo is set here.
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType())) {
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}

	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDCarriedAmmo(CarriedAmmo);
	}
}

void UCombatComponent::EquipSecondaryWeapon(AWeapon* WeaponToEquip)
{
	SecondaryWeapon = WeaponToEquip;
	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
	AttachActorToBackpack(WeaponToEquip);
	SecondaryWeapon->SetOwner(Character);
}

void UCombatComponent::AttachActorToBackpack(AActor* ActortoAttach)
{
	if (Character == nullptr || Character->GetMesh() == nullptr || ActortoAttach == nullptr || EquippedWeapon == nullptr) return;

	const USkeletalMeshSocket* BackpackSocket = Character->GetMesh()->GetSocketByName(FName("BackpackSocket"));
	if (BackpackSocket) {
		BackpackSocket->AttachActor(ActortoAttach, Character->GetMesh());
	}
}

void UCombatComponent::ServerAddAmmo_Implementation(EWeaponType WeaponType, int32 AmmoAmount)
{
	if (Character == nullptr || EquippedWeapon == nullptr) { return; }
	if (CarriedAmmoMap.Contains(WeaponType)) {
		if (EquippedWeapon) {
			CarriedAmmoMap[EquippedWeapon->GetWeaponType()] = EquippedWeapon->GetAmmo() + AmmoAmount;
			EquippedWeapon->AddAmmo(CarriedAmmoMap[EquippedWeapon->GetWeaponType()]);
		}
	}
}

void UCombatComponent::PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount)
{
	//ServerAddAmmo(WeaponType, AmmoAmount);
	if (Character == nullptr || EquippedWeapon == nullptr) { return; }

	if (CarriedAmmoMap.Contains(WeaponType)) {
		if (EquippedWeapon) {
			EquippedWeapon->AddAmmo(AmmoAmount);
		}
	}
}

// RepNotifier for our Equipped Weapon Animation.
void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Character) {
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);	// This sets our WeaponState to "Equipped".

		const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));	// We'll define a HandSocket variable of type USkeletalMeshSocket.
		// Then, we'll retrieve our RightHandSocket that made and defined.
		if (HandSocket) {
			HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());	// In the HandSocket that we created to hold our weapon, we'll attach our "EquippedWeapon" reference.	
			// AttachActor requires the Actor that attaches to the socket and the skeletal mesh component associated with the socket.
		}
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;
		EquippedWeapon->SetHUDAmmo();
	}
}

void UCombatComponent::OnRep_SecondaryWeapon()
{
	if (SecondaryWeapon && Character) {
		SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
		AttachActorToBackpack(SecondaryWeapon);
	}
}

// This is our TraceUnderCrosshairs With Notes.
/*	**** NOTES FOR RAY CASTING ****
// This is where we're going to have our projectiles launch from the center of the screen.
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
		FVector Start = CrosshairWorldPosition; // The starting position for our line trace, we actually need to start infront of our Character.
		
		// We're gonna start the Line Trace infront of our Character.
		if (Character) {
			float DistanceToCharacter = (Character->GetActorLocation() - Start).Size();	// Size() Pretty much Makes this a vector. 			
			Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);
			// DrawDebugSphere(GetWorld(), Start, 16.f, 12, FColor::Red, false);
		}

		// This is how long we want our Raytrace to be.
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

		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 2.0f);

		// DrawDebugLine(GetWorld(), CrosshairWorldPosition, End, FColor::Orange);
		// This sets the color of our Crosshair depending on what our TraceHitResult returns.

		if (TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UInteractWithCrosshairsInterface>()) { // Checking to see if that actor is valid and if that Actor implements "UInteractWithCrosshairsInterface" Interface.			
			HUDPackage.CrosshairsColor = FLinearColor::Red;
			
			AActor* HitActor = TraceHitResult.GetActor();			
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s, Class: %s"), *HitActor->GetName(), *HitActor->GetClass()->GetName());
			if (TraceHitResult.GetActor()->Implements<UInteractWithCrosshairsInterface>()) {
				UE_LOG(LogTemp, Warning, TEXT("True"));
				HUDPackage.CrosshairsColor = FLinearColor::Red;
			}
			

			UE_LOG(LogTemp, Warning, TEXT("Character Blocking: %d"), TraceHitResult.GetActor());
		}
		else {
			HUDPackage.CrosshairsColor = FLinearColor::White;
		}
		
	}
}
*/

// Cleaner Version of our TraceUnderCrosshairs Function
void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;	
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(ViewportSize);	
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f); 

	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),  
		CrosshairLocation, 
		CrosshairWorldPosition,	
		CrosshairWorldDirection	
	);

	if (bScreenToWorld) {	
		FVector Start = CrosshairWorldPosition; 

		if (Character) {
			float DistanceToCharacter = (Character->GetActorLocation() - Start).Size();		
			Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);
			// DrawDebugSphere(GetWorld(), Start, 16.f, 12, FColor::Red, false);
		}

		FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;	

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,	
			Start,	
			End,
			ECollisionChannel::ECC_Visibility
		);	

		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 2.0f);

		// DrawDebugLine(GetWorld(), CrosshairWorldPosition, End, FColor::Orange);
		// This sets the color of our Crosshair depending on what our TraceHitResult returns.

		if (TraceHitResult.GetActor() && TraceHitResult.bBlockingHit && TraceHitResult.GetActor()->Implements<UInteractWithCrosshairsInterface>()) {
			HUDPackage.CrosshairsColor = FLinearColor::Red;
		}
		else {
			HUDPackage.CrosshairsColor = FLinearColor::White;
		}

	}
}

// This is how we set the CrossHairs to our screen when picking up our Gun.
void UCombatComponent::SetHUDCrossHairs(float DeltaTime)
{
	if (Character == nullptr || Character->Controller == nullptr) { return; }

	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;	// We'll only cast once so it's not so expensive.
	if (Controller) {
		HUD = HUD == nullptr ? Cast<ABlasterHUD>(Controller->GetHUD()) : HUD;	// If Hud is not nullptr, then we just set it equal to itself, so we should only cast once.
		if (HUD) { // If we do have an equipped Weapon.			

			if (EquippedWeapon) {
				HUDPackage.CrosshairsCenter = EquippedWeapon->CrosshairsCenter;
				HUDPackage.CrosshairsLeft = EquippedWeapon->CrosshairsLeft;
				HUDPackage.CrosshairsRight = EquippedWeapon->CrosshairsRight;
				HUDPackage.CrosshairsTop = EquippedWeapon->CrosshairsTop;
				HUDPackage.CrosshairsBottom = EquippedWeapon->CrosshairsBottom;
			}
			else {	// If we don't have an equipped Weapon.
				HUDPackage.CrosshairsCenter = nullptr;
				HUDPackage.CrosshairsLeft = nullptr;
				HUDPackage.CrosshairsRight = nullptr;
				HUDPackage.CrosshairsTop = nullptr;
				HUDPackage.CrosshairsBottom = nullptr;
			}
			// Calculate Crosshair Spread
			// We need to create a value that will always be somewhere between 0 and 1 for our character's movement. 0 meaning the slowest and 1 meaning the fastest for our character movement.
			// Map our Speed [0, 600] -> [0, 1]
			FVector2D WalkSpeedRange(0.f, Character->GetCharacterMovement()->MaxWalkSpeed);	// We want to map our Character's Movement to our Velocity Range.
			FVector2D VelocityMultiplierRange(0.f, 1.f);	// Here's the value that we want to clamp on our Character's Movement Speed.
			FVector Velocity = Character->GetVelocity();
			Velocity.Z = 0.f;

			// This will automatically convert our Walkspeed to our Velocity Range. "Velocity.Size()" will get the magnitude of our Vector, the length of it.
			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

			if (Character->GetCharacterMovement()->IsFalling()) {	// This is when we are in air.
				// Interpolate and Spread the Crosshairs more smoothly. FInterpto Requires a current value, a target value (2.25 is the Max Spread that we want),
				// the DeltaTime, and the InterpSpeed.
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
			}
			else {	// This is when we are on the ground.
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.F, DeltaTime, 15.f);	// When we hit the ground, we wanna Interp pretty fast back to 0 length.
			}

			if (bAiming) {
				CrosshairAimFactor = FMath::FInterpTo(CrosshairVelocityFactor, 0.5f, DeltaTime, 15.f);
			}
			else {
				CrosshairAimFactor = FMath::FInterpTo(CrosshairVelocityFactor, 0.f, DeltaTime, 15.f);
			}

			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 3.25f);

			// Now this is how much our Crosshair can spread.
			HUDPackage.CrosshairSpread =
				0.25f +
				CrosshairVelocityFactor +
				CrosshairShootingFactor +
				CrosshairInAirFactor -
				CrosshairAimFactor;

			HUD->SetHUDPackage(HUDPackage);
		}
	}
}

void UCombatComponent::InterpFOV(float DeltaTime)
{
	if (EquippedWeapon == nullptr) { return; }

	// Receiving the value of Camera so we can manipulate it.
	if (bAiming) {
		CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedWeapon->GetZoomedFOV(), DeltaTime, EquippedWeapon->GetZoomInterpSpeed());	// Interpolating for Aiming.
	}
	else {
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);	// Interpolating back to Aiming.
	}

	// Set the FOV on the Actual Camera.
	if (Character && Character->GetFollowCamera()) {
		Character->GetFollowCamera()->SetFieldOfView(CurrentFOV);
	}

}

FVector UCombatComponent::GetCenterOfCameraTransform()
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
	);

	FVector Endpoint = CrosshairWorldDirection * 10000.f;

	return Endpoint;
}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	// On the client, this aiming animation will work first on that client before it's sent through an RPC reaching the server. Therefore, the aiming
	// mechanics on the client will execute first before reaching the server and this is fine.
	bAiming = bIsAiming;

	// We don't need to check if the character is on the server, because no matter what, it will be executed on the server even if were on the client.
	// So calling this function on the server means we're replicating it to all other clients. 
	ServerSetAiming(bAiming);

	// If our Character is Aiming, set the Walk Speed to Aim Speed / else leave it at BaseWalkSpeed.
	if (Character) {
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}
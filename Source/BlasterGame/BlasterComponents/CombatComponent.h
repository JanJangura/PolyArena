// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlasterGame/HUD/BlasterHUD.h"
#include "BlasterGame/Weapon/WeaponTypes.h"
#include "CombatComponent.generated.h"

#define TRACE_LENGTH 80000.f

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTERGAME_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	// "friend" This is going to give our UBlasterCharacter Class full access to this class (including protected and private members). 
	friend class ABlasterCharacter;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// This is a Server function. This function is executed on the server. The server uses it to determine how to replicate the properties to the clients.
	// You can also define conditions under which specific properties should be replicated using replication conditions like COND_None, COND_OwnerOnly, COND_SkipOwner, etc.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Within this EquipWeapon, we need a Weapon to equip, we can forward a class instance of AWeapon class.
	void EquipWeapon(class AWeapon* WeaponToEquip);	

	void SwapWeapons();

	void PickupAmmo(class AAmmoPickup* AmmoClass, EWeaponType WeaponType, int32 AmmoAmount, FTransform AmmoTransform);

	void FireButtonPressed(bool bPressed);

protected:
	virtual void BeginPlay() override;

	// This will handle everything, server and client side for aiming.
	void SetAiming(bool bIsAiming);	

	// This is our Server RPC. We can pass data in parameters across the network with RPCs. To make this a Server RPC, we need to add the Macro "UFUNCTION(Server, Reliable)".
	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();

	UFUNCTION()
	void OnRep_SecondaryWeapon();

	void Fire();

	// This Server RPC is in Protected so we can change the value of this with child classes. This Server RPC is for firing the weapon.
	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget);	// FVector_NetQuantize will take our FVector and round it down to whole numbers and send across the network. 
											// Lowers bandwidth by cutting off information before sending data out. It is a child class of type FVector, but for network.

	// This is our NetMulticast Server RPC, we will call this on the Server, which we'll link it with the Server RPC.
	UFUNCTION(NetMulticast,Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	// Line Trace
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	// Setting the CrossHairs based on the weapon we are using.
	void SetHUDCrossHairs(float DeltaTime);

	// This is TEMPORARY
	UFUNCTION(Server, Reliable)
	void ServerAddAmmo(class AAmmoPickup* AmmoClass, EWeaponType WeaponType, int32 AmmoAmount, FTransform AmmoTransform);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAddAmmo(class AAmmoPickup* AmmoClass);

	void EquipPrimaryWeapon(AWeapon* WeaponToEquip);
	AWeapon* PrimaryWeapon;
	void EquipSecondaryWeapon(AWeapon* WeaponToEquip);

	void AttachActorToBackpack(AActor* ActortoAttach);

private:
	// We need this instance because we'll be referring back and forth. This also stops us from continously casting to our Character.
	UPROPERTY()
	class ABlasterCharacter* Character;	
	UPROPERTY()
	class ABlasterPlayerController* Controller;
	UPROPERTY()
	class ABlasterHUD* HUD;
	UPROPERTY()
	class ALobbyBlasterHUD* LobbyBlasterHUD;

	// This is the variable for the weapon in which the Blaster Character currently has equipped.
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)	// We need to add this "Replicated" key term to indicate that this is Replicated across all clients. Otherwise we're only setting it on the Server.
	AWeapon* EquippedWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon)
	AWeapon* SecondaryWeapon;

	UPROPERTY(Replicated)	// This is so all clients can see whomever is aiming.
	bool bAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;

	
	//	****** HUD and Crosshairs ******
	FHUDPackage HUDPackage;

	FVector HitTarget;

	float CrosshairVelocityFactor; // Character Movement Speed Factor for Spreading the Crosshair.

	float CrosshairInAirFactor;	// Character Movement InAir Factor for Spreading the Crosshair.

	float CrosshairAimFactor; // This will Decrease the Spread of Crosshair when Aiming.

	float CrosshairShootingFactor;	// This will Increase the Spread of Crosshair when Shooting.

	//	****** End of HUD and Crosshairs ******



	// ****** Aiming and FOV ******
	float DefaultFOV;	// Field of View when Not Aiming, set to the camera's base FOV in BeginPlay.

	UPROPERTY(EditAnywhere, category = Combat)
	float ZoomedFOV = 30.f;

	float CurrentFOV; // Current Field of View.

	UPROPERTY(EditAnywhere, category = Combat)
	float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);	// This will hand zooming when we're aiming.

	FVector GetCenterOfCameraTransform();

	// **** AutomaticFire ****
	FTimerHandle FireTimer;

	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();

	void PauseButtonToggle();

	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;	// This is the Carried Ammo for the currently equiped Weapon that the player is holding.

	UFUNCTION()
	void OnRep_CarriedAmmo();

	// TMap cannot be replicated. This is because TMap is a Hash Algorithm that will figure out a way to associate the key (EWeapnType) with the value (int32). 
	// When we add a key valued pair to the Ammo Map, everytime we attemp to look up the value using the key, our team map will run a hash function that will come up with an index.
	// We don't want to map this, we only replicate the value that we're currently using, which is CarriedAmmo.
	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int32 StartingARAmmo = 30;

	UPROPERTY(EditAnywhere)
	int32 StartingRocketAmmo = 0;

	UPROPERTY(EditAnywhere)
	int32 StartingPistolAmmo = 30;

	UPROPERTY(EditAnywhere)
	int32 StartingShotgunAmmo = 0;

	void InitializeCarriedAmmo();

	class ABlasterGameMode* GameMode;

public:	
	bool ShouldSwapWeapons();

	FORCEINLINE AWeapon* GetSecondaryWeapon() const { return SecondaryWeapon; }
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "BlasterGame/Interfaces/InteractWithCrosshairsInterface.h"
#include "BlasterGame/Weapon/WeaponTypes.h"
#include "Blueprint/UserWidget.h"
#include "BlasterGame/BlasterTypes/TurningInPlace.h"
#include "BlasterCharacter.generated.h"

// Declaring a Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftGame);

UCLASS()
class BLASTERGAME_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlasterCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Replication STEP 2. This is a const function that we need to override in any class where we plan on replicating variables.
	// This is a Server function. This function is executed on the server. The server uses it to determine how to replicate the properties to the clients.
	// You can also define conditions under which specific properties should be replicated using replication conditions like COND_None, COND_OwnerOnly, COND_SkipOwner, etc.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Initializing before the game is built. 
	// This function will give us acccess to our Combat Component.
	virtual void PostInitializeComponents() override;	// This inherited function allows us to initalize things within it before the game is built.

	// Play Fire Montage
	void PlayFireMontage(bool bAiming);

	// Play Elim Montage
	void PlayElimMontage();

	virtual void OnRep_ReplicatedMovement() override;

	void Elim(bool bPlayerLeftGame);

	// Handles what happens when the player gets eliminated.
	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim(bool bPlayerLeftGame);

	UPROPERTY(Replicated)
	bool bDisableGameplay = false; // This is a Boolean that we'll use to turn off the Movement controls while in the Cooldown State, but we'll only allow access to look around.

	void SpawnDefaultWeapon();

	UFUNCTION(Server, Reliable)
	void ServerLeaveGame();

	// CUSTOM Delegate 
	FOnLeftGame OnLeftGame;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float value);
	void LookUp(float value);
	virtual void Jump() override;
	void EquipButtonPressed();	// This is an action mapping callback, so we don't have an input parameter.
	void AimButtonPressed();
	void AimButtonReleased();
	void AimOffset(float DeltaTime);
	void CalculateAO_Pitch();
	void SimProxiesTurn();
	void FireButtonPressed();
	void FireButtonReleased();
	void PlayHitReactMontage(); // Play Hit React Montage
	virtual void Destroyed() override;

	UFUNCTION()	// You will never get your callbacks called in response to a damage event if your receive damage function is not a UFUNCTION
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	void PollInit(); // As soon as our PlayerState is valid, we'll initialize our HUD using any relevant Data that exists on the PlayerState.
	void RotateInPlace(float DelaTime);

	void UpdateHUDHealth();
	void DropOrDestroyWeapon(AWeapon* Weapon);
	void DropOrDestroyWeapons();

private:
	// Setting up our Camera System
	UPROPERTY(VisibleAnywhere, Category = Camera) 
	class USpringArmComponent* CameraBoom;		// Setting up Third Person Perspective with this.

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;		// Setting this up so camera can follow character.

	// Displaying Text over our Character.
	// We can't read a variable that is private in blueprints, unless we proved this specifier meta = (AllowPrivateAccess = "true").
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;


	/* Replication STEP 1 - Before Change
	// We want to store the weapon that we are overlapping.
	// We want this variable to be replicated, which means when it changes on the server, then we'd like it to change on all clients.
	// This will designate the overlapping weapon to be a replicated variable.
	// Essentially, this AWeapon ptr "OverlappingWeapon" will eventually help us find the weapon class that we are overlapping.
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)	
	class AWeapon* OverlappingWeapon;	
	*/

	// Now in order to designate this RepNotify to be called whenever OverlappingWeapon gets replicated, we take our weapon UPROPERTY and we add the specifier
	// "ReplicatedUsing" and then we specify the RepNotifier "OnRep_OverlappingWeapon". With this UPROPERTY specifier, we will now have OnRep_OverlappingWeapon be 
	// called on the Client when OverlappingWeapon replicates to that client (this is that "GetLifetimeReplicatedProps()" function).
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	// RepNotify is a function that will be called whenever the variable value gets replicated. These are void functions beginning with "OnRep" followed by an underscore
	// and the variable that we are replicating "_OverlappingWeapon". Nothing gets passed in as RepNotifies are automatically called when the variable gets replicated,
	// except only the Type of the replicated variable can be passed in (in this case "AWeapon*" is our type). This is so we can know when to close the pickup Widget.
	// These also have to be UFUNCTION(), so we need this Macro in order to use these RepNotifiers. 
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);	// This "LastWeapon" that's passed in parameters, is going to be the last value before the replication happened.
	// When we add an input parameter to a RepNotifier, then what get's passed in when it gets called upon variable replication is going to be the last value before the 
	// replication happened.

	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;	// The instance to our class CombatComponent

	// Defining our Remote Procedure Calls (RPC) here.
	// Call this on the client, but execute this on the Server. We call these Server RPCs.
	// We have to specify if the RPC is reliable or unreliable. If the RPC is reliable, then the RPC is guaranteed to execute. Whereas the unreliable RPC could potentially be dropped.
	// Under the hood of RPC reliable, the client will receive confirmation when the server recieves the RPC, and if it doesn't send confirmation, the RPC will be sent again. 
	// This should only be used precisely. We need this though for equipping because we need to guarantee that a player can equip a weapon if it is possible here.
	// As we send information from client to server, the information is sent in packets. Packets are unit of 
	// information sent across the network, and packets can be dropped. 
	UFUNCTION(Server, Reliable) 
	void ServerEquippedButtonPressed();

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
	void TurningPlace(float DeltaTime);

	void HideCameraIfCharacterClose();

	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;

	float TimeSinceLastMovementReplication;
	float CalculateSpeed();

	// ***** PLAYER HEALTH *****
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health = 100.f;	// We want to Replicate this.

	UFUNCTION()
	void OnRep_Health();

	UPROPERTY()
	class ABlasterPlayerController* BlasterPlayerController;

	bool bElimmed = false;

	FTimerHandle ElimTimer;
	FTimerHandle HealthRegenTimerHandle;

	UPROPERTY(EditDefaultsOnly) // EditDefaultsOnly allows us to Edit this but only on the Default Character.
	float ElimDelay = 3.f;

	void ElimTimerFinished();

	bool bLeftGame = false;

	void RegenerateHealth(); // Function to handle health regeneration

	UPROPERTY()
	class ABlasterPlayerState* BlasterPlayerState;

	// Default Weapon
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AWeapon> DefaultWeaponClass;

	float LastHealth;

public:	
	void UpdateWeaponIcon();
	/* Replication STEP 4. FORCEINLINE is a simple getter.
	// We need a public setter for our "OverlappingWeapon" variable.
	// We'll need a FORCEINLINE Macro and this will be void. It will take in AWeapon ptr and we'll set our replicating variable "OverlappingWeapon = Weapon".
	// This FORCEINLINE Macro suggets compiler that it should inline the function, meaning that the function's code will be inserted directly at each point 
	// the function is called, rather than having the overhead of a function call. This can improve performance, especially for simple, frequently-called functions.
	// As soon as the value of overlapping weapon changes, then it will replicate, which means the variable will be set on all Client BlasterCharacters. 
	// Replication only works when the variable is changed, so it won't update every frame or update. Only when OverlappingWeapon changes on the Server do it
	// change in the Client. This is the function we'll call from our Weapon.cpp file. 
	FORCEINLINE void SetOverlappingWeapon(AWeapon* Weapon) { OverlappingWeapon = Weapon; }
	*/

	// This is the more indepth build of our SetOverlappingWeapon function.
	void SetOverlappingWeapon(AWeapon* Weapon);

	// This is for our AnimInstance, we need to return a bool to transition State Machines.
	bool IsWeaponEquipped();

	// This is for our Character's Aiming 
	bool IsAiming();

	AWeapon* GetEquippedWeapon();

	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Jump")
	float JumpHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Jump")
	float GravitySetting;

	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }

	UPROPERTY(EditAnywhere, Category = "Animation")
	class UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class UAnimMontage* ElimMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class USoundBase* HitReactSound;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class USoundBase* HitReactSoundTwo;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<USoundBase*> JumpSoundEffects;

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FVector GetHitTarget() const;

	FVector GetCenterOfCameraTransform();

	void PauseButtonPressed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<class AAmmoPickup> SpawnAmmoPickup;

	void SpawnAmmoPacks();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float HealthRegenAmount = 5.0f; // Amount of health to regenerate per interval

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float HealthRegenInterval = 1.0f; // Time interval for health regeneration

	void UpdateHUDAmmo();

	FORCEINLINE bool IsElimmed() const { return bElimmed; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }
	FORCEINLINE EWeaponType GetWeaponType() const;
};

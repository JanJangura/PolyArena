// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


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

protected:
	virtual void BeginPlay() override;

	// This will handle everything, server and client side for aiming.
	void SetAiming(bool bIsAiming);	

	// This is our Server RPC. We can pass data in parameters across the network with RPCs. To make this a Server RPC, we need to add the Macro "UFUNCTION(Server, Reliable)".
	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

private:
	// We need this instance because we'll be referring back and forth. This also stops us from continously casting to our Character.
	class ABlasterCharacter* Character;	

	// This is the variable for the weapon in which the Blaster Character currently has equipped.
	UPROPERTY(Replicated)	// We need to add this "Replicated" key term to indicate that this is Replicated across all clients. Otherwise we're only setting it on the Server.
	AWeapon* EquippedWeapon;

	UPROPERTY(Replicated)	// This is so all clients can see whomever is aiming.
	bool bAiming;

public:	
	

		
};

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

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// "friend" This is going to give our UBlasterCharacter Class full access to this class (including protected and private members). 
	friend class ABlasterCharacter; 

	// Within this EquipWeapon, we need a Weapon to equip, we can forward a class instance of AWeapon class.
	void EquipWeapon(class AWeapon* WeaponToEquip);	

protected:
	virtual void BeginPlay() override;

private:
	// We need this instance because we'll be referring back and forth. This also stops us from continously casting to our Character.
	class ABlasterCharacter* Character;	

	// This is the variable for the weapon in which the Blaster Character currently has equipped.
	AWeapon* EquippedWeapon;

public:	
	

		
};

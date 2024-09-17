// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterAnimInstance.h"
#include "BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// This is how we get our Blaster Character.
	BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
}

void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BlasterCharacter == nullptr) {
		BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
	}

	if (BlasterCharacter == nullptr) {
		return;
	}

	// Get our character's speed
	FVector Velocity = BlasterCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size(); // This gets the magnitude of our Vector.

	bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling(); // This returns a bool to lets us know if we are in the air. 

	// This just lets us know if we're adding input, like if we're pressing keys to move. We'll get this from our character movement. 
	// If our current acceleration size() is greater then zero, return true, else return false.
	bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true:false;

	// This checks if our EquippedWeapon from our BlasterCharacter is true.
	bWeaponEquipped = BlasterCharacter->IsWeaponEquipped();

	// We're setting the bAiming variable depending if our BlasterCharacter returns true when the player is aiming.
	// This checks if BlasterCharacter has Combat Component and if the player CombatComponent "bAiming" variable is true, if it is, then we set this
	// local variable "bAiming" to true, and then whenever this is true, the AnimInstance will switch to the aiming animation that we set up.
	bAiming = BlasterCharacter->IsAiming();
}

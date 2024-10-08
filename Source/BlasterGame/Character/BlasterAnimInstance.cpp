// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterAnimInstance.h"
#include "BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BlasterGame/Weapon/Weapon.h"

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

	// This returns our Equipped Weapon from our Linked Character.
	EquippedWeapon = BlasterCharacter->GetEquippedWeapon();

	// We're setting the bAiming variable depending if our BlasterCharacter returns true when the player is aiming.
	// This checks if BlasterCharacter has Combat Component and if the player CombatComponent "bAiming" variable is true, if it is, then we set this
	// local variable "bAiming" to true, and then whenever this is true, the AnimInstance will switch to the aiming animation that we set up.
	bAiming = BlasterCharacter->IsAiming();

	// This is our ENUM Class
	TurningInPlace = BlasterCharacter->GetTurningInPlace();

	bElimmed = BlasterCharacter->IsElimmed();

	// ******************************** Leaning and Strafing ********************************
	// ******** Logic for STRAFING ********
	
	// In order to get an offset, we need to know which direction we're moving and which direction we're aiming (or where the controller is pointing in).
	FRotator AimRotation = BlasterCharacter->GetBaseAimRotation();	// This GetBaseAimRotation is a GLOBAL Rotation of the world, it's not local.

	// MakeRotFromX returns an F-Rotator and takes a vector which is a direction. We'll get the Velocity for this direction.
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BlasterCharacter->GetVelocity());	// Rotation that corresponds to our movement. Returns our Direction Vector. 
	
	// This is how we get the Delta between the two Yaw Movements
	// We want smooth transition from strafing left to strafing right and vice versa, so we'll just interpolate the lean value itself and gets around the interpolation 
	// time in the blend space. 
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
	YawOffset = DeltaRotation.Yaw;

	// UE_LOG(LogTemp, Warning, TEXT("AimRotaion Yaw %f: "), AimRotation.Yaw);

	// ******** Logic for LEANING ********
	// Lean has to do with our Delta Yaw Rotation between the Character itself and its own rotation from the previous frame.

	// This is simply the rotation of the Root Component or the Capsule Component.
	CharacterRotationLastFrame = CharacterRotation;	// The last frame of our Character's Rotation
	CharacterRotation = BlasterCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);	// Get the Delta between the two.
	const float Target = Delta.Yaw / DeltaTime;	// We'll scale this up and also be proportionate to DeltaTime.

	// Interp this so there's no Jerking in our lean. Interp our current value of our lean variable to our target and we need to pass in DeltaTime and also a speed. 
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);	
	Lean = FMath::Clamp(Interp, -90.f, 90.f);	// Setting our Lean Value, we'll clamp this in between 90 and -90 degrees.

	// We didn't need to Replicate this because the function "GetBaseAimRotation()" is set on our clients and on the server, and we're using values that are already replicated.

	// Acquiring our Character's Yaw information
	AO_Yaw = BlasterCharacter->GetAO_Yaw();
	AO_Pitch = BlasterCharacter->GetAO_Pitch();

	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && BlasterCharacter->GetMesh()) {
		static const FName LeftHandSocketName("LeftHandSocket");
		static const FName RightHandBoneName("hand_r");
		static const FName MuzzleFlash("MuzzleFlash");

		// We'll retrieve the transform of our LeftHandSocket in WorldSpace.
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(LeftHandSocketName, ERelativeTransformSpace::RTS_World);

		FVector OutPosition;	// This OutPosition will receieve date from the TransformToBoneSpace() function. This is the location of our LeftHandSocket Transformed to our Hand R BoneSpace.
		FRotator OutRotation;	// This OutRotation will receieve date from the TransformToBoneSpace() function. This is the Rotation of our LeftHandSocket Transformed to our Hand R BoneSpace.

		// We want the WorldSpace of our LeftHandTransform and converted into BoneSpace. We retrieve the Character's Mesh and call upon TransformToBoneSpace.
		// We want to transform our LeftHandSocket on the Weapon into BoneSpace for one of the bones on our Character Mesh, which is the Right Hand. This is so
		// our LeftHandSocket on the Weapon is relative to the Right Hand.
		BlasterCharacter->GetMesh()->TransformToBoneSpace(RightHandBoneName, LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));

		if (BlasterCharacter->IsLocallyControlled()) {
			// This let's us know that we are Locally Controlled and it's use is for inside of BP_Anim_Instance.
			bLocallyControlled = true;

			// Get the Transform of our "hand_r" Socket, Then rotate that arm towards the Rotation of the target that BlasterCharacter is shooting at.
			FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(RightHandBoneName, ERelativeTransformSpace::RTS_World);

			if (BlasterCharacter->GetHitTarget() != FVector::ZeroVector) {
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - BlasterCharacter->GetHitTarget()));
				RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaTime, 20.f);
			}
			else {
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() - BlasterCharacter->GetCenterOfCameraTransform());
				RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaTime, 20.f);
			}

			/*	*** This is to draw RayCast Lines out of our Gun and from the center of our screen.
			// Acquring the Transform of our MuzzleTip
			FTransform MuzzleTipTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(MuzzleFlash, ERelativeTransformSpace::RTS_World);

			// We can get the X-Axis accordingly from FRotationMatrix.
			FVector MuzzleX(FRotationMatrix(MuzzleTipTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X));
			DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), MuzzleTipTransform.GetLocation() + MuzzleX * 1000.f, FColor::Red);
			DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), BlasterCharacter->GetHitTarget(), FColor::Orange);
			*/
		}
		
	}
}

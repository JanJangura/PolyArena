// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Projectile.h"

void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);	// This allows us to use functionality within the base (Parent) Class.

	if (!HasAuthority()) { return; }	// If it's not on the Server, then just return.

	APawn* InstigatorPawn = Cast<APawn>(GetOwner());

	// We'll spawn the projectile at the location of our muzzle flash socket. We can store this in a local variable.
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));	// We need to get the mesh, and then we'll need to get the Socket by Name.
	if (MuzzleFlashSocket) {
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());	// So now we know where we can spawn our projectile.
		// From Muzzle flash socket to hit location from TraceUnderCrosshairs.

		FVector ToTarget;
		FRotator TargetRotation;

		if (HitTarget.IsZero()) {
			ToTarget = GetCenterOfCameraTransform();
		}
		else {
			ToTarget = HitTarget - SocketTransform.GetLocation();				
		}
		TargetRotation = ToTarget.Rotation();

		if (ProjectileClass && InstigatorPawn) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = InstigatorPawn;
			UWorld* World = GetWorld();
			if (World) {
				World->SpawnActor<AProjectile>(
					ProjectileClass,
					SocketTransform.GetLocation(),
					TargetRotation,
					SpawnParams
				);
			}
		}
	}
}

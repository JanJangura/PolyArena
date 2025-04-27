// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "BlasterGame/Weapon/WeaponTypes.h"
#include "AmmoPickup.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API AAmmoPickup : public APickup
{
	GENERATED_BODY()
	
protected:
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	FVector ThisActorLocation;

	FTimerHandle RespawnTimerHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Ammo Properties")
	int32 AmmoIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo Properties")
	FTransform AmmoLocation;

	UFUNCTION()
	void StartRespawnTimer();

	UFUNCTION()
	void FinishRespawn();

	UPROPERTY(EditAnywhere, Category = "Ammo Properties")
	float AmmoRespawnCooldown = 7.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmmoAmount = 10;
};

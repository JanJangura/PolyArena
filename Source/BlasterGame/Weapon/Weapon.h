// Fill out your copyright notice in the Description page of Project Settings.

// ***** NOTE TO REMEMBER ***** 
// This is a replicating actor. If the weapon were not a replicating actor, then we couldn't have variables replicate on it.
// The variable "bReplicates = true;"  allows us to have replicating variables.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

// Setting an Enum for the state of the weapon. This is a scoped Enum, meaning we're going to make an Enum Class called EWeaponState and use uint8. This is so
// our enum constants are unsigned eight bit integers and this is also going to be a blueprint type.
UENUM(BlueprintType)
enum class EWeaponState :uint8 {						
	EWS_Initial UMETA(DisplayName = "InitialState"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_Max UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class BLASTERGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Step 1 for Replication: Declare the Server Function "GetLifetimeReplicatedProps()"
	// This is the Server Function that we let the Server know which variable we're replicating to the Owning Client. We'll be overriding this now and Unreal will do the rest.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// This function is needed when we need to show and now to show our pickup text.
	void ShowPickupWidget(bool bShowWidget);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

private:
	// This is how we setup our Skeletal Mesh Component.
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	// Overlap volume so we can equip this weapon.
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* AreaSphere;

	// Step 2 for Replication: Allowing Weapon State to be replicated.
	// Remember to add "ReplicatedUsing = OnRep_WeaponState", this allows our "WeaponState" variable to be replicated.
	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	// Step 3 for Replication: Use a rep_notifier.
	// This is the rep_notifier for when the weapon state changes.
	UFUNCTION()
	void OnRep_WeaponState();

	// This is to show our Pick Up Text.
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* PickupWidget;

public:	
	// Getters and Setters
	void SetWeaponState(EWeaponState State);	// A function that uses a Public setter in setting the Weapon State.
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
};

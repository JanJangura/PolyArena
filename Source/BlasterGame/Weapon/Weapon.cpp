// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// This is where we set our Weapon Replicate to true. The actor class has a default bReplicates boolean to let the Engine know that it's a replicate.
	bReplicates = true;

	// Constructing our Weapon Mesh.
	// This is essentially 
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(WeaponMesh); // This is our root

	//////// *WEAPON COLLISION* ////////
	// We can also set the CollisionResponse of the weapon mesh, such as dropping it and it bouncing off of the walls or ground.
	// "ToAllChannels" means everything. Then we'll set ECollisionResponse to ECR_Block. This will block everything.
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);	

	// Once the weapon is dropped, we want it to ignore our character's collision. We'll set the channel to the "Pawn" and then set the Response to "Ignore".
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);	

	// The weapon does not start off in this state. It starts in a state where we can walk up to it and pick it up.
	// So in the actual start of the weapon, we'll have the weapon mesh to have its collision disabled.
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//////// *WEAPON AREASPHERE* ////////
	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);

	// This Sphere is going to be what we use to detect overlaps with characters. This differes in Multiplayer and Single player games.
	// In a multiplayer game, it's important that mechanics like this is done on the server only. So we only want our AreaSphere to detect overlap events
	// if we're on the server. 
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);	// This Sphere overlap is set to Ignore all channels.
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);				// The Sphere Collision is also set to NoCollision. 
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// If we're on the server, then we'll enable our AreaSphere Collision.
	// We can check to see if we have authority here and then later we can add overlap functions, we can bind those here and begin play if we're on the server.
	// Essentially, this checks to see if we are on the server by checking if we have authority. 
	if (HasAuthority()) {	// Alternate way of writing HasAuthority() function: GetLocalRole() == ENetRole::ROLE_Authority
		// HasAuthority() Checks the LocalRole of this Weapon Actor and if it has the role "Authority", then HasAuthority() will return true.

		// We'll enable our collision here. 
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		// Then we can set our Collision Reponse Channel to Pawn, and overlap it with Response.
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		// *Last Note Here* - This if-statement can only happen if our weapon is a replicating actor. This means that the server will be in charge of all weapon objects.
		// So for our weapon to have authority only on the server, we need to set our Weapon Actor to Replicate.
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


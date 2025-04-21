// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "BlasterGame/Character/BlasterCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"
#include "BlasterGame/PlayerController/BlasterPlayerController.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// This is where we set our Weapon Replicate to true. The actor class has a default bReplicates boolean to let the Engine know that it's a replicate.
	bReplicates = true;	// So now we can have replicating variables. 

	// Constructing our Weapon Mesh.
	// This is the Default Scene Root Component within Blueprints.
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh); // We set this WeaponMesh to become our Root

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

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (PickupWidget) {
		PickupWidget->SetVisibility(false);
	}

	// If we're on the server, then we'll enable our AreaSphere Collision.
	// We can check to see if we have authority here and then later we can add overlap functions, we can bind those here and begin play if we're on the server.
	// Essentially, this checks to see if we are on the server by checking if we have authority. 
	if (HasAuthority()) {	// Alternate way of writing HasAuthority() function: GetLocalRole() == ENetRole::ROLE_Authority
		// HasAuthority() Checks the LocalRole of this Weapon Actor and if it has the role "Authority", then HasAuthority() will return true. This allows us the server
		// to know that this object is on the server, and if it is then the server has control over it. 

		// We'll enable our collision here. 
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		// Then we can set our Collision Reponse Channel to Pawn, and overlap it with Response.
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		// Calling the DELEGATES for our AreaSphere Overlaps
		// This is a Delegate that we call when the overlap happens.
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
		// This is a Delegate that we call when the overlap ends.
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);


		// *Note Here* - This if-statement can only happen if our weapon is a replicating actor. This means that the server will be in charge of all weapon objects.
		// So for our weapon to have authority only on the server, we need to set our Weapon Actor to Replicate.
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// We can register our variables that we want to replicate ("WeaponState").

	// In DOREPLIFETIME, we want to pass in the class and the variable.
	DOREPLIFETIME(AWeapon, WeaponState);	// Now we've registered WeaponState in GetLifeTimeReplicatedProps, but we also need to make sure to add replicated to the UProperty.
	DOREPLIFETIME(AWeapon, Ammo);
}

// Remember, this function only gets called on the server because it has the role Authority.
void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// This casts to our ABlasterCharacter class targeting the OtherActor.
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter) {
		// Replication STEP 5. When this actor finally overlaps with our character, we'll set the replicated variable "OverlappingWeapon" from BlasterCharacter, 
		// using our SetOverlappingWeapon() function, to this class. This function only happens on the Server.
		BlasterCharacter->SetOverlappingWeapon(this);
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter) {
		BlasterCharacter->SetOverlappingWeapon(nullptr); // Instead of setting it to this class, we just set it to a nullptr.
	}
}

void AWeapon::AddAmmo(int32 AmmoToAdd)
{
	Ammo += AmmoToAdd;
	SetHUDAmmo();
}

void AWeapon::SetHUDAmmo()
{
	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
	if (BlasterOwnerCharacter) {
		BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(BlasterOwnerCharacter->Controller) : BlasterOwnerController;
		if (BlasterOwnerController) {
			BlasterOwnerController->SetHUDWeaponAmmo(Ammo);
		}
	}
}

void AWeapon::SpendRound()
{
	--Ammo;
	SetHUDAmmo();
}

void AWeapon::OnRep_Ammo()
{
	SetHUDAmmo();
}

void AWeapon::OnRep_Owner()
{
	Super::OnRep_Owner();
	if (Owner == nullptr) {
		BlasterOwnerCharacter = nullptr;
		BlasterOwnerController = nullptr;
	}
	else {
		BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(Owner) : BlasterOwnerCharacter;
		if (BlasterOwnerCharacter && BlasterOwnerCharacter->GetEquippedWeapon() && BlasterOwnerCharacter->GetEquippedWeapon() == this) {
			SetHUDAmmo();
		}
	}
}

void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;

	// This is on the server.
	switch (WeaponState) {
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false); // Now that we picked up this weapon, we don't need to prompt the "Press E" widget anymore. We'll turn it off here.
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Removing the Collision off of the weapon now on the server.
		WeaponMesh->SetSimulatePhysics(false);	// This is how we allow our Gun to fall to the ground.
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EWeaponState::EWS_EquippedSecondary:
		OnEquippedSecondary();
		break;
	case EWeaponState::EWS_Dropped:
		if (HasAuthority()) {	// Need to make sure that Spawning the AreaSphere is on the server and not on the client.
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		WeaponMesh->SetSimulatePhysics(true);	// This is how we allow our Gun to fall to the ground.
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetLinearDamping(1.0f);    // Slows down sliding
		WeaponMesh->SetAngularDamping(5.0f);   // Slows down rolling


		// If someone walks up and tries to equip the gun when Physics is enabled, it won't work because Physics is enabled. So before we equip
		// the weapon, we need to disable the gun's physics.
		break;
	}
}

// Our Rep_Notifier, this is for the Clients.
void AWeapon::OnRep_WeaponState()
{
	switch (WeaponState) {
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false); // Now that we picked up this weapon, we don't need to prompt the "Press E" widget anymore. We'll turn it off here.
		WeaponMesh->SetSimulatePhysics(false);	// This is how we allow our Gun to fall to the ground.
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	case EWeaponState::EWS_EquippedSecondary:
		OnEquippedSecondary();
		break;
	case EWeaponState::EWS_Dropped:
		WeaponMesh->SetSimulatePhysics(true);	// This is how we allow our Gun to fall to the ground.
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetLinearDamping(1.0f);    // Slows down sliding
		WeaponMesh->SetAngularDamping(5.0f);   // Slows down rolling

	}
}

void AWeapon::OnEquippedSecondary()
{
	ShowPickupWidget(false); // Now that we picked up this weapon, we don't need to prompt the "Press E" widget anymore. We'll turn it off here.
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Removing the Collision off of the weapon now on the server.
	WeaponMesh->SetSimulatePhysics(false);	// This is how we allow our Gun to fall to the ground.
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::ShowPickupWidget(bool bShowWidget)
{
	if (PickupWidget) {
		PickupWidget->SetVisibility(bShowWidget);
	}
}

// This should only be called on the Server.
void AWeapon::Fire(const FVector& HitTarget)
{
	if (FireAnimation) {
		WeaponMesh->PlayAnimation(FireAnimation, false);	// Play the animation, and then true or false if we want to loop the animation.
	}

	SpendRound();
}

void AWeapon::Dropped()
{
	SetWeaponState(EWeaponState::EWS_Dropped);
	
	// We should Detach the weapon from the Character. Skeletal Mesh Components have this functionality. 
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);	// This is a struct, and within this struct we need to set rules for the Detachment.
	WeaponMesh->DetachFromComponent(DetachRules);	// We can Detach this Mesh using the DetachRules.
	SetOwner(nullptr);	// Now that the weapon is unequipped, there is no owner.
	BlasterOwnerCharacter = nullptr;
	BlasterOwnerController = nullptr;
}
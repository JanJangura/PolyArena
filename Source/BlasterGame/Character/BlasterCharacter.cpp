// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "BlasterGame/Weapon/Weapon.h"
#include "BlasterGame/BlasterComponents/CombatComponent.h"

// Sets default values
ABlasterCharacter::ABlasterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Constructing our Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));	
	CameraBoom->SetupAttachment(GetMesh()); // Attaching our Camera Boom to our Character's Mesh. This is because when we crouch, our character's capsule changes size.
	CameraBoom->TargetArmLength = 600.f;	// Length of the Camera Boom.
	CameraBoom->bUsePawnControlRotation = true; // This is set to true so we can rotate our Camera Boom along with our controller, when adding our mouse input. 

	// Creating our Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);	// Setting our Camera attached to our CameraBoom at the Socket.
	FollowCamera->bUsePawnControlRotation = false;	// We don't need pawn rotation because our Camera is already attached to the CameraBoom which already has one.

	// Variables
	bUseControllerRotationYaw = false; // Setting our bUseControllerRotationYaw to false so our character doesn't turn in the direction of where our camera is facing.
	GetCharacterMovement()->bOrientRotationToMovement = true; 

	// This is to Display our Network Role
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	// Constructing our Combat instance. This is going to handle all combat related functionality for our character, so we're going to have variables on this combat component
	// that will be replicated. First, our Combat Component itself needs to be replicated.
	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);	// Now our Combat Component is designated to be a replicating component.
}

// This is a Server Function that we need to let the Server know which variable we're replicating to the Owning Client.
// The GetLifetimeReplicatedProps() function is not something you call manually. Instead, it is automatically invoked by Unreal Engine as part of the replication process.
void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*Replication STEP 3 - Deleted because we won't need it anymore.
	// This is where we need to register our overlapping weapon variable to be replicated.
	// DOREPLIFETIME is a Macro and within it we need to specify the "ABlasterCharacter" class, that's the class that that has the replicated variable and we
	// also specify the replicated variable "OverlappingWeapon". DOREPLIFETIME_CONDITION: This macro is used to register the OverlappingWeapon property for 
	// replication with a specific condition, in this case, COND_OwnerOnly. This means that the OverlappingWeapon property will only be replicated to the owning client.
	DOREPLIFETIME(ABlasterCharacter, OverlappingWeapon);	// This variable starts off uninitialized, it will be null until we set it.
	*/

	// When someone else overlaps with a weapon and we don't need to see the pickup widget, we can control which clients will have this variable replicated to them.
	// We can do this by adding a condition when we register this variable to be replicated. To use a replicating condition, we need the DOREPLIFETIME_CONDITION Macro.
	// There are ENUM Constants that we can use to specify conditions. In this case, we want the COND_OwnerOnly condition, which means OverlappingWeapon will only replicate
	// to the client that owns the BlasterCharacter. So we should only see this pickup widget on the client that overlapped with the weapon. 
	// *REMEMBER* This is called whenever the variable OverlappingWeapon changes value.
	DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon, COND_OwnerOnly); // This variable starts off uninitialized, it will be null until we set it.
}

// Called when the game starts or when spawned
void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input. This is Character function.
void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// This is our Action Mapping. We'll bind a basic jump to our character here. BindAction(ActionName, EInputEvent (We'll select pressed), user object, function).
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	// This is our Axis Mapping. This is how we would bind our movements to our Axis Mappings.
	PlayerInputComponent->BindAxis("MoveForward", this, &ABlasterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABlasterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABlasterCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABlasterCharacter::LookUp);
	
	// This is our Equipped Action Mapping and we'll bind it to our EquipButtonPressed() function.
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ABlasterCharacter::EquipButtonPressed);	
}

void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat) {	// This should be valid because of our "friend" declaration within the CombatComponent. Which allows us full access to that Class.
		Combat->Character = this;	// Then we can set the Instance Ptr Character to be this class.
	}
}

////////////// MOVEMENTS //////////////
void ABlasterCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f) {
		// We want to move forward in our controller's forward direction and not the character's. 

		// We need to find out which way is forward. 
		// This YawRotation type FRotator is 0.f for the pitch and roll and it's yaw is the Controller's rotation Yaw.
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f); 
		
		// We need a Direction and to find this we need to know our Forward Vector. We can get the Forward Vector from our YawRotation, using the FRotationMatrix
		// FRotation Matrix is a mathematical construct called a matrix, you can create a rotation matrix from a rotator and then it contains information that we'll need.
		// We can call GetUnitAxis, specifying the X-Axis, and this will return us an FVector that represents the direction of this YawRotation. Since YawRotation has 0 pitch & roll,
		// then this vector is parallel to the ground, pounted in the direction corresponding to the yaw of our controller. 
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X)); // "X" will retrieve our Forward Vector.
		// This is the direction we want to move forward in.
		AddMovementInput(Direction, Value); // This only moves the character in this direction. Speed and accel are retrieved from the character movement component.
	}
}

void ABlasterCharacter::MoveRight(float Value)
{
	// Notes for understanding these codes are similar in ABlasterCharacter::MoveForward(float Value).
	if (Controller != nullptr && Value != 0.f) {

		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y)); // "Y" will retrieve our Right Vector.
	
		AddMovementInput(Direction, Value); 
	}
}

void ABlasterCharacter::Turn(float value)
{
	AddControllerYawInput(value); // Value will determine how fast we move our mouse and AddControllerYawInput is going to add Yaw to our controller rotation.
}

void ABlasterCharacter::LookUp(float value)
{
	AddControllerPitchInput(value); // Value will determine how fast we move our mouse and AddControllerPitchInput is going to add Pitch to our controller rotation.
}

// When the E key is pressed, EquipButtonPressed() function will only call the EquipWeapon() function if we have authority. 
void ABlasterCharacter::EquipButtonPressed()
{
	// Keep in mind when pressing "E" is done on both Client and Server, but things like equpping Weapon should only be done on the Server. 
	if (Combat) {	
		if (HasAuthority()) { // Only the server is allowed to call the weapon, "HasAuthority" checks if it's on the server.
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else {	// Now if we press the E key that's not on the server, then we know that we're on the client. Here, we need to send the RPC.
			ServerEquippedButtonPressed(); // We also don't need the "_Implementation" key word here, this is only for us when defining the function.
		}

		// Remote Procedure Calls (RPC) are functions that we can call on one machine and have them executed on another machine. So we can call RPC from the client, 
		// and have that function executed on the server. We'll use this by creating an RPC that we can call from a client and execute on the server to allow a client 
		// to pick a weapon.
	}
}

// If we don't have authority when pressing E, we want to send this RPC so that the server can call the EquipWeapon() function for us (client).
// Everything must be ran through the server, therefore we need the server to do all the work for us. Therefore we need this RPC to allow us to equip weapons.
// This "_Implementation" is needed because Unreal Engine will create the actual function definition for this function and all we have to do is create the implementation
// which will define what happens when the RBC is executed on the Server.
void ABlasterCharacter::ServerEquippedButtonPressed_Implementation()	
{
	if (Combat) {	// There's no need to check for authority because we know that a Server RPC will only be executed on the server.
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

// This is our RepNotifier that will showcase our PickupWidget to Client-Only.
void ABlasterCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	// This is our RepNotify. This is called whenever the OverlappingWeapon variable has changed as it has just been replicated. So we can show the pickup widget in here.
	// *IMPORTANT NOTE* - RepNotifiers are not called on the Server, so we won't see this PickUpWidget on the Server. Server only works one way, Server to Client (Server -> Client).
	// Since OverlappingWeapon will never be replicated to the Server, the Server will never get the RepNotifier. So this also brings another issue of the RepNotifier not
	// showing on the server side.
	// This AWeapon* LastWeapon variable will be the last value of our Overlapping Weapon before the variable was replicated. So before the OverlappingWeapon value changes,
	// we should check the last value before the change. If that value is not null, then we can access that last weapon and hide the pickup widget.
	if (OverlappingWeapon) {
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeapon) {	// This LastWeapon will retain our AWeapon Class so we can still access the ShowPickupWidget() function and turn off the Widget.
		LastWeapon->ShowPickupWidget(false);
	}
}

// This is the condition in which we scoped it to being the player that is hosting the game on the Server and will showcase our PickupWidget here for that player specifically.
void ABlasterCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	// This is called as soon as the function is called, but when we leave the AreaSphere and call this function again and set Weapon to nullptr from the Weapon Class, it still
	// calls this function, which checks this OverlappingWeapon if it's valid, which it should be because we never changed the value of it until after call this if-statement, 
	// and then we can turn the ShowPickupWidget off before we change it's value to null. Therefore it will always work.
	if (OverlappingWeapon) {
		OverlappingWeapon->ShowPickupWidget(false);
	}
	// This is called from the server, this is because of how we constructed it from the Weapon.cpp class. Now we set the OverlappingWeapon to our Weapon Class.
	OverlappingWeapon = Weapon;
	// We still need to know if we're actually on the character that's controlling the Pawn. We can check with this IsLocallyControlled() function that returns a bool.
	// IsLocallyControlled() will be true if this function is called on the character that's actually being controlled.
	if (IsLocallyControlled()) { // This allows us to know that we're on the character that is being controlled by the player that is hosting the game on the server.
		// We also know that the overlapping weapon will not be replicated because we have our condition of OwnerOnly from GetLifeTimeReplicatedProps() function.
		// This also tells us that none of the clients will be owner of this character because it's locally controlled on the server.
		// So all we gotta do is show the PickupWidget here!
		if (OverlappingWeapon) {
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

bool ABlasterCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);	// We'll check if we have a weapon equipped and return true or false.
}

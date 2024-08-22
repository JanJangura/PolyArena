// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"

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
}

// Called when the game starts or when spawned
void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
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

// Called every frame
void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
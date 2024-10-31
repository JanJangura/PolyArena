// Fill out your copyright notice in the Description page of Project Settings.
// IMPORTANT NOTE, Replication only works one way. From Server to Client, you have to find a way for the Client to replicate to the Server. We can do this with Rep Notify
// and sending server RPC.

#include "BlasterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "BlasterGame/Weapon/Weapon.h"
#include "BlasterGame/BlasterComponents/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BlasterAnimInstance.h"
#include "BlasterGame/BlasterGame.h"
#include "BlasterGame/HUD/BlasterHUD.h"
#include "BlasterGame/PlayerController/BlasterPlayerController.h"
#include "BlasterGame/Pickups/AmmoPickup.h"
#include "TimerManager.h"
#include "BlasterGame/GameMode/BlasterGameMode.h"
#include "BlasterGame/PlayerState/BlasterPlayerState.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABlasterCharacter::ABlasterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Adjusting our Spawn Location
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

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

	// Jump and Gravity Mechanics
	JumpHeight = 800.f;
	GravitySetting = 1.f;
	GetCharacterMovement()->JumpZVelocity = JumpHeight;
	GetCharacterMovement()->GravityScale = GravitySetting;

	// Default Weapon
	DefaultWeaponClass = LoadObject<UClass>(nullptr, TEXT("/Game/BP_Shooter_Character/Blueprints/Weapon/BP_Pistol.BP_Pistol_C"));

	// Blocking Camera Issue 
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);	// Capsule will now not block the camera.
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh); // Defaulting our Character to this Custom Collision that we created.
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);	// Mesh will now not block the camera.
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	// Default Value of TurningInPlace 
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;

	// Setting the Net Frequency to a Default Value
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

	// Initializes the FireMontage for us because it doesnt work in blueprints.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireMontageAsset(TEXT("AnimMontage'/Game/BP_Shooter_Character/Animation/FireWeapon.FireWeapon'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactMontageAsset(TEXT("AnimMontage'/Game/BP_Shooter_Character/Animation/HitReact.HitReact'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ElimMontageAsset(TEXT("AnimMontage'/Game/BP_Shooter_Character/Animation/Elim.Elim'"));

	bool bMontagesAreValid = FireMontageAsset.Succeeded() 
		&& HitReactMontageAsset.Succeeded() 
		&& ElimMontageAsset.Succeeded();

	if (bMontagesAreValid)
	{
		FireWeaponMontage = FireMontageAsset.Object;
		HitReactMontage = HitReactMontageAsset.Object;
		ElimMontage = ElimMontageAsset.Object;
		// UE_LOG(LogTemp, Warning, TEXT("Elim loaded successfully!"));
	}
	else
	{
		FireWeaponMontage = nullptr;
		HitReactMontageAsset = nullptr;
		ElimMontage = nullptr;
		//UE_LOG(LogTemp, Error, TEXT("Failed to load FireWeaponMontage."));
	}
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
	DOREPLIFETIME(ABlasterCharacter, Health);
	DOREPLIFETIME(ABlasterCharacter, bDisableGameplay);
}

// Called when the game starts or when spawned
void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Player Equips Default Weapon and update Ammo Hud.
	//UE_LOG(LogTemp, Warning, TEXT("DefaultWeaponClass: %s"), DefaultWeaponClass != nullptr ? TEXT("True") : TEXT("False"));
	SpawnDefaultWeapon();
	if (Combat && Combat->PrimaryWeapon) {
		UpdateWeaponIcon();
	}
	UpdateHUDAmmo();

	// Blocking Camera Issue 
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);	// Capsule will now not block the camera.
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh); // Defaulting our Character to this Custom Collision that we created.
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);	// Mesh will now not block the camera.
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	if (HasAuthority()) {
		OnTakeAnyDamage.AddDynamic(this, &ABlasterCharacter::ReceiveDamage);	// This is called because its linked with ApplyDamage on the Authority Character.
	}

	Health = MaxHealth;
	UpdateHUDHealth();
}

void ABlasterCharacter::Destroyed()
{
	Super::Destroyed();

	ABlasterGameMode* BlasterGameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));

	bool bMatchNotInProgress = BlasterGameMode && BlasterGameMode->GetMatchState() != MatchState::InProgress;
	if (Combat && Combat->EquippedWeapon && bMatchNotInProgress) {
		Combat->EquippedWeapon->Destroy();
	}
}

// Called every frame
void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateInPlace(DeltaTime);
	HideCameraIfCharacterClose();
	PollInit();
}

void ABlasterCharacter::RotateInPlace(float DeltaTime)
{
	if (bDisableGameplay) { 
		bUseControllerRotationYaw = false;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return; 
	}

	if (GetLocalRole() > ENetRole::ROLE_SimulatedProxy && IsLocallyControlled()) {	// This Greater Sign means that we only want to play AimOffSet with any roles above SimulatedProxy().
		AimOffset(DeltaTime);
	}
	else {
		TimeSinceLastMovementReplication += DeltaTime;
		if (TimeSinceLastMovementReplication > 0.25f) {
			OnRep_ReplicatedMovement();
		}
		CalculateAO_Pitch();
	}
}

// This is how we'll Equip our Default Weapon
void ABlasterCharacter::SpawnDefaultWeapon()
{
	ABlasterGameMode* BlasterGameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)); // If not on the server, it'll return null. 
	UWorld* World = GetWorld();

	// This checks to see if we're on the server and that we're on the Game Mode: BlasterGameMode, otherwise it'll return null. 
	if (BlasterGameMode && World && !bElimmed && DefaultWeaponClass) { 
		AWeapon* StartingWeapon = World->SpawnActor<AWeapon>(DefaultWeaponClass); // We'll spawn the Weapon.
		StartingWeapon->bDestroyWeapon = true; // This is so we can Destroy our Default Weapon.

		if (Combat) {
			Combat->EquipWeapon(StartingWeapon); // This is how we'll Equip the Weapon.
		}
	}
}

// RepNotifier that Unreal Made for our Characters Movements
void ABlasterCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();
	SimProxiesTurn();
	TimeSinceLastMovementReplication = 0.f;
}

// This is the Server that we'll call from the GameMode, then on the server we'll call MulticastElim() because that's including Clients and Server, but we only want the Server which is here.
void ABlasterCharacter::Elim()
{
	DropOrDestroyWeapons();

	MulticastElim();

	// Here is our Timer that we would like to wait, clarified by ElimDelay, and then we call the function within it "ElimTimerFinished" so in there we can Respawn. 
	GetWorldTimerManager().SetTimer(
		ElimTimer,
		this,
		&ABlasterCharacter::ElimTimerFinished,
		ElimDelay
	);
}

// Remember, this is an Multicast RPC
void ABlasterCharacter::MulticastElim_Implementation()
{
	if (BlasterPlayerController) {
		BlasterPlayerController->SetHUDWeaponAmmo(0);
	}

	bElimmed = true;
	PlayElimMontage();

	// Disable Character Movement
	GetCharacterMovement()->DisableMovement();	// Disables Movement
	GetCharacterMovement()->StopMovementImmediately();	// Disables the rotation of the Character
	if (BlasterPlayerController) {
		bDisableGameplay = true;	// Disable Input stops the character from Shooting the Gun.
	}
	// Disable Collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// Disables the Capsule Component
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// Disables the Mesh Component
}

void ABlasterCharacter::ElimTimerFinished()
{
	// Get the GameMode so we can Request for a Respawn.
	ABlasterGameMode* BlasterGameMode = GetWorld()->GetAuthGameMode<ABlasterGameMode>();
	if (BlasterGameMode) {
		BlasterGameMode->RequestRespawn(this, Controller);
	}
}

void ABlasterCharacter::DropOrDestroyWeapons()
{
	if (Combat) {
		if (Combat->EquippedWeapon) {
			DropOrDestroyWeapon(Combat->EquippedWeapon);
		}
		if (Combat->SecondaryWeapon) {
			DropOrDestroyWeapon(Combat->SecondaryWeapon);
		}
	}
}

void ABlasterCharacter::DropOrDestroyWeapon(AWeapon* Weapon)
{
	if (Weapon == nullptr) return;

	if (Weapon->bDestroyWeapon) {
		Weapon->Destroy();
	}
	else {
		Weapon->Dropped();
	}
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
	
	// This is our Equipped Action Mapping and we'll bind it to our functions that we want to call for each action.
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ABlasterCharacter::EquipButtonPressed);	
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ABlasterCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ABlasterCharacter::AimButtonReleased);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABlasterCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABlasterCharacter::FireButtonReleased);

	// This is our Pause Menu Logic
	PlayerInputComponent->BindAction("PauseButton", IE_Pressed, this, &ABlasterCharacter::PauseButtonPressed);
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
	if (bDisableGameplay) return; // This is to Disable Movement in the Cooldown State.

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
	if (bDisableGameplay) return;

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

void ABlasterCharacter::Jump()
{
	if (bDisableGameplay) return;

	Super::Jump();

}

// When the E key is pressed, EquipButtonPressed() function will only call the EquipWeapon() function if we have authority. 
void ABlasterCharacter::EquipButtonPressed()
{
	if (bDisableGameplay) return;

	// Keep in mind when pressing "E" is done on both Client and Server, but things like equpping Weapon should only be done on the Server. 
	if (Combat) {	

		ServerEquippedButtonPressed();
		/*
		if (HasAuthority()) { // Only the server is allowed to call the weapon, "HasAuthority" checks if it's on the server.
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else {	// Now if we press the E key that's not on the server, then we know that we're on the client. Here, we need to send the RPC.
			ServerEquippedButtonPressed(); // We also don't need the "_Implementation" key word here, this is only for us when defining the function.
		}
		*/

		// Remote Procedure Calls (RPC) are functions that we can call on one machine and have them executed on another machine. So we can call RPC from the client, 
		// and have that function executed on the server. We'll use this by creating an RPC that we can call from a client and execute on the server to allow a client 
		// to pick a weapon.
	}
}

void ABlasterCharacter::AimButtonPressed()
{
	if (bDisableGameplay) return;

	if (Combat) {
		Combat->SetAiming(true);
		Combat->SetAiming(true);
	}
}

void ABlasterCharacter::AimButtonReleased()
{
	if (bDisableGameplay) return;

	if (Combat) {
		Combat->SetAiming(false);
	}
}

// Calculating our Speed.
float ABlasterCharacter::CalculateSpeed() {
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	return Velocity.Size();
}

void ABlasterCharacter::OnRep_Health()
{
	UpdateHUDHealth();
	PlayHitReactMontage();
}

void ABlasterCharacter::UpdateHUDHealth()
{
	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;

	if (HasAuthority()) {
		//UE_LOG(LogTemp, Warning, TEXT("BlasterPlayerController: %s"), BlasterPlayerController != nullptr ? TEXT("PlayController Valid") : TEXT("PlayerController is Not Valid"));
	}
	
	if (BlasterPlayerController) {
		BlasterPlayerController->SetHUDHealth(Health, MaxHealth);
	}
}

void ABlasterCharacter::UpdateHUDAmmo()
{
	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;

	if (BlasterPlayerController && Combat && Combat->EquippedWeapon) {
		BlasterPlayerController->SetHUDCarriedAmmo(Combat->CarriedAmmo);
		BlasterPlayerController->SetHUDWeaponAmmo(Combat->EquippedWeapon->GetAmmo());
	}
}

EWeaponType ABlasterCharacter::GetWeaponType() const
{
	if (Combat && Combat->PrimaryWeapon) {
		//UE_LOG(LogTemp, Warning, TEXT("WeaponType: %d"), static_cast<int>(Combat->EquippedWeapon->GetWeaponType()));
		return Combat->PrimaryWeapon->GetWeaponType();
	}
	else {
		return EWeaponType::EWT_None;
	}
}

void ABlasterCharacter::PollInit()
{
	if (BlasterPlayerState == nullptr) {
		// This allows us to avoid Casting
		BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
		if (BlasterPlayerState) {
			BlasterPlayerState->AddToScore(0.f);
			BlasterPlayerState->AddToDefeats(0);
		}
	}
}

// This only happens when our character is not moving, when the character is still in idle position.
void ABlasterCharacter::AimOffset(float DeltaTime)
{
	if (Combat && Combat->EquippedWeapon == nullptr) { return; }

	float Speed = CalculateSpeed(); // This gets the magnitude of our Vector.
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir) {	// We're standing still and not jumping.
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		// This is the Delta between CurrentAimRotation and StartingAimRotation. Difference in Rotation between our Starting and Current AimRotation.
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;	// This is to Rotate our AimOffSet with our Mouse because remember it gets passed into BlasterAnimInstance.

		if (TurningInPlace == ETurningInPlace::ETIP_NotTurning) {
			InterpAO_Yaw = AO_Yaw;
		}
		
		bUseControllerRotationYaw = true;

		if (!IsAiming()) {

			TurningPlace(DeltaTime);
		}
		else {
			AO_Yaw = 0.f;
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			// bUseControllerRotationYaw = false; Setting our bUseControllerRotationYaw to false so our character doesn't turn in the direction of where our camera is facing. 
		}
	}

	if (Speed > 0.f || bIsInAir) {	// Running, or Jumping.
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);	// This is store for every single frame when running or in the air if we have a weapon.
		AO_Yaw = 0.f;	// This Disables the rotation of AimOffSet because now we're moving and now AimOffSet Animation won't move.
		bUseControllerRotationYaw = true; // Allows the character to rotate with the way the Camera is facing.
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	CalculateAO_Pitch();
}

void ABlasterCharacter::TurningPlace(float DeltaTime)
{
	// Remember that Yaw is our Horizonal Direction in which the player faces.
	if (AO_Yaw > 90.f) {
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AO_Yaw < -90.f) {
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}

	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning) {
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.f);
		AO_Yaw = InterpAO_Yaw;

		if (FMath::Abs(AO_Yaw) < 15.f) {
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

void ABlasterCharacter::CalculateAO_Pitch()
{
	AO_Pitch = GetBaseAimRotation().Pitch;

	// Unreal Engine Compresses Angles from 0 to 360, so only in positive numbers, when negative numbers are sent, they are masked and turned into positive angles. We fix that here.
	if (AO_Pitch > 90.f && !IsLocallyControlled()) {
		// Map Pitch from the range [270, 360) tp [-90, 0).
		FVector2D InRange(270.f, 360.f);	// Declaring Range
		FVector2D OutRange(-90.f, 0.f);		// Declaring the Range we would like to match.
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);	// Applying the correction Here.
	}
}

void ABlasterCharacter::SimProxiesTurn()
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) { return; }
	float Speed = CalculateSpeed();
}

void ABlasterCharacter::PlayFireMontage(bool bAiming)
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && FireWeaponMontage) {
		AnimInstance->Montage_Play(FireWeaponMontage);	// Play this Montage
		FName SectionName;
		SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");	// If we're aiming, return the name "RifleAim" Montage Section, else return "RifleHip" Montage Secion.
		AnimInstance->Montage_JumpToSection(SectionName);	// Jump to the Anim Montage depending on the bool above and play that animation.
	}
}

void ABlasterCharacter::PlayElimMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && ElimMontage) {
		AnimInstance->Montage_Play(ElimMontage);	// Play this Montage
	}
}

void ABlasterCharacter::PlayHitReactMontage()
{
	bool bCombat = Combat
		&& Combat->EquippedWeapon;

	if (!bCombat) {
		return; 
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HitReactMontage) {
		AnimInstance->Montage_Play(HitReactMontage);	// Play this Montage
		FName SectionName("FromFront");
		AnimInstance->Montage_JumpToSection(SectionName);	// Jump to the Anim Montage depending on the bool above and play that animation.
	}
}

void ABlasterCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	UpdateHUDHealth();
	PlayHitReactMontage();

	if (Health <= 0.f) {
		GetWorld()->GetTimerManager().ClearTimer(HealthRegenTimerHandle);
		ABlasterGameMode* BlasterGameMode = GetWorld()->GetAuthGameMode<ABlasterGameMode>();
		if (BlasterGameMode) {
			BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
			ABlasterPlayerController* AttackerController = Cast<ABlasterPlayerController>(InstigatorController);

			if (BlasterPlayerController && AttackerController) {
				bElimmed = true;
				BlasterGameMode->PlayerEliminated(this, BlasterPlayerController, AttackerController);
			}

		}
	}
	else {
		GetWorld()->GetTimerManager().SetTimer(HealthRegenTimerHandle, this, &ABlasterCharacter::RegenerateHealth, HealthRegenInterval, true);
	}
}

void ABlasterCharacter::RegenerateHealth()
{
	float CurrentHealth = Health;

	Health = FMath::Clamp(Health + HealthRegenAmount, 0.f, MaxHealth);
	UpdateHUDHealth();

	// If health reaches max, stop regeneration
	if (Health >= MaxHealth) {
		GetWorld()->GetTimerManager().ClearTimer(HealthRegenTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Health is at maximum. Stopping regeneration."));
	}
	else if (Health <= 0.f) {
		GetWorld()->GetTimerManager().ClearTimer(HealthRegenTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Health is zero or below. Stopping regeneration."));
	}
}

void ABlasterCharacter::UpdateWeaponIcon()
{
	if (!Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is null!"));
		return;
	}
	if (IsLocallyControlled() && Combat && Combat->PrimaryWeapon) {

		ABlasterPlayerController* BlasterController = Cast<ABlasterPlayerController>(Controller);
		if (BlasterController && Combat->PrimaryWeapon) {
			BlasterController->UpdateWeaponIcon(Combat->PrimaryWeapon->GetWeaponType());

			if (HasAuthority()) {
				UE_LOG(LogTemp, Warning, TEXT("Server WeaponType: %d"), static_cast<int>(Combat->PrimaryWeapon->GetWeaponType()));
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Client WeaponType: %d"), static_cast<int>(Combat->PrimaryWeapon->GetWeaponType()));
			}
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Client  Character->IsLocallyControlled is invalid"));
	}
}

void ABlasterCharacter::SpawnAmmoPacks()
{
	if (!HasAuthority()) { return; }
	SpawnAmmoPickup = AAmmoPickup::StaticClass();

	if (SpawnAmmoPickup)
	{
		// Get the current actor location (your character's location)
		FVector ActorLocation = GetActorLocation();

		// Add an offset to drop items around the character
		FVector Offset(0.f, 0.f, 500.f);  // Adjust X, Y, Z for different spawn locations

		// Combine location and offset
		FVector SpawnLocation = ActorLocation + Offset;

		// Random rotation for the spawned item
		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();


		UWorld* World = GetWorld();
		if (World) {
			World->SpawnActor<AAmmoPickup>(
				SpawnAmmoPickup,
				SpawnLocation,
				SpawnRotation
			);
			UE_LOG(LogTemp, Warning, TEXT("reached"));
			UE_LOG(LogTemp, Warning, TEXT("SpawnAmmoPickup Class: %s"), *SpawnAmmoPickup->GetName());

		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("SpawnAmmoPickup NOT reached"));
	}
}

void ABlasterCharacter::PauseButtonPressed()
{
	if (bDisableGameplay) return;

	if (Controller && HasAuthority()) {
		Combat->PauseButtonToggle();
	}
}

void ABlasterCharacter::FireButtonPressed()
{
	if (bDisableGameplay) return;

	if (Combat) {
		Combat->FireButtonPressed(true);
		//UE_LOG(LogTemp, Warning, TEXT("TRUE"));
	}
}

void ABlasterCharacter::FireButtonReleased()
{
	if (bDisableGameplay) return;

	if (Combat) {
		Combat->FireButtonPressed(false);
		//UE_LOG(LogTemp, Warning, TEXT("FALSE"));
	}
}

// If we don't have authority when pressing E, we want to send this RPC so that the server can call the EquipWeapon() function for us (client).
// Everything must be ran through the server, therefore we need the server to do all the work for us. Therefore we need this RPC to allow us to equip weapons.
// This "_Implementation" is needed because Unreal Engine will create the actual function definition for this function and all we have to do is create the implementation
// which will define what happens when the RBC is executed on the Server.
void ABlasterCharacter::ServerEquippedButtonPressed_Implementation()	
{
	if (Combat) {	// There's no need to check for authority because we know that a Server RPC will only be executed on the server.
		if (OverlappingWeapon) {
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else {
			if (Combat->ShouldSwapWeapons()) {
				Combat->SwapWeapons();
				UpdateWeaponIcon();
				//UE_LOG(LogTemp, Warning, TEXT("WeaponType: %d"), static_cast<int>(PrimaryWeaponType));
			}
		}

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

// This hides our Character when our Camera gets pushed too close too our Character, meaning that we're backing up on something.
void ABlasterCharacter::HideCameraIfCharacterClose()
{
	// We only want what is Locally Controlled, the player that is controlling the computer.
	if (!IsLocallyControlled()) { return; }

	// Pretty Much if the CameraLocation - ActorLocation is < then CameraThreshold, we'll hide the Character.
	if ((FollowCamera->GetComponentLocation() - GetActorLocation()).Size() < CameraThreshold) {
		GetMesh()->SetVisibility(false);
		if (Combat && Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponMesh()) {
			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = true;
		}
	}
	else {
		GetMesh()->SetVisibility(true);
		if (Combat && Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponMesh()) {
			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = false;
		}
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
	// We'll check if we have a weapon equipped and return true or false.
	return (Combat && Combat->EquippedWeapon);	
}

bool ABlasterCharacter::IsAiming()
{
	// We'll check if combat is valid and if Combat->bAiming is true.
	return (Combat && Combat->bAiming);
}

AWeapon* ABlasterCharacter::GetEquippedWeapon()
{
	if (Combat == nullptr) { return nullptr; }
	return Combat->EquippedWeapon;
}

FVector ABlasterCharacter::GetHitTarget() const
{
	if (Combat == nullptr) { return FVector(); }
	else {
		return Combat->HitTarget;
	}
}

FVector ABlasterCharacter::GetCenterOfCameraTransform()
{
	if (Combat == nullptr) { return FVector(); }
	return Combat->GetCenterOfCameraTransform();
}
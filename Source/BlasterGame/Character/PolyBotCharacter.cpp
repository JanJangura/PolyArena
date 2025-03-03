// Fill out your copyright notice in the Description page of Project Settings.


#include "PolyBotCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

APolyBotCharacter::APolyBotCharacter()
	:Super()
{
	// Set the new Skeletal Mesh
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> PolyBotMesh(TEXT("/Game/BP_PolyBot_Character/Asset/Testing/PolyBot11.PolyBot11"));

	// Ensure Movement Component is enabled (redundant but good practice)
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    GetCharacterMovement()->bAlwaysCheckFloor = true;
    GetCharacterMovement()->bOrientRotationToMovement = true;

}

void APolyBotCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //UE_LOG(LogTemp, Warning, TEXT("Character Speed: %f"), GetVelocity().Size());

    if (GetCharacterMovement()->IsMovingOnGround())
    {
        //UE_LOG(LogTemp, Warning, TEXT("Character is ON GROUND"));
        //UE_LOG(LogTemp, Warning, TEXT("Acceleration: %s"), *GetCharacterMovement()->GetCurrentAcceleration().ToString());
        //UE_LOG(LogTemp, Warning, TEXT("Is Character Possessed? %s"), GetController() ? TEXT("YES") : TEXT("NO"));

    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Character is FLOATING or NOT on solid ground"));
    }
}

void APolyBotCharacter::BeginPlay()
{
    Super::BeginPlay();
    

}

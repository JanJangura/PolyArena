// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlasterCharacter.generated.h"

UCLASS()
class BLASTERGAME_API ABlasterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlasterCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float value);
	void LookUp(float value);

private:
	// Setting up our Camera System
	UPROPERTY(VisibleAnywhere, Category = Camera) 
	class USpringArmComponent* CameraBoom;		// Setting up Third Person Perspective with this.

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;		// Setting this up so camera can follow character.

	// Displaying Text over our Character.
	// We can't read a variable that is private in blueprints, unless we proved this specifier meta = (AllowPrivateAccess = "true").
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

public:	
	// Simple Getters and Setters

};

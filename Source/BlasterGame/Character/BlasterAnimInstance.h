// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BlasterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API UBlasterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// We are overriding some inherited functions.
	virtual void NativeInitializeAnimation() override; // This is much like Begin Play, it is called when the game loads.
	virtual void NativeUpdateAnimation(float DeltaTime) override; // This one is like tick. 

private:
	// We need a variable that stores our character. This character will be using this Anim Instance.
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class ABlasterCharacter* BlasterCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABlasterPlayerController();
	void SetHUDWeaponAmmo(int32 Ammo);
	virtual void Tick(float DeltaTime) override;
	void SetHUDHealth(float Health, float MaxHealth);
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	class ABlasterHUD* BlasterHUD;

	class ABlasterCharacter* Character;
	AHUD* CurrentHUD;

	/*
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<ABlasterHUD> SpecialHUDClass;
	*/

protected:
	virtual void BeginPlay() override;
	 
public:
	// void SetNewHUD();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BlasterGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API UBlasterGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	TArray<class APlayerState*> PlayerStates;

	void AddPlayerStates(APlayerState* PlayerState);
};

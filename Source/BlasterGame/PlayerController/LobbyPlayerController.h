// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterPlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API ALobbyPlayerController : public ABlasterPlayerController
{
	GENERATED_BODY()
	
public:
	class ALobbyBlasterHUD* LobbyBlasterHUD;

	void UpdatePlayerListInfo(TArray<FString> PlayerName);

protected:
	virtual void BeginPlay() override;
};

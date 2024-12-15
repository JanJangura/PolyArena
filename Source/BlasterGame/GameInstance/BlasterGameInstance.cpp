// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameInstance.h"
#include "GameFramework/PlayerState.h"

void UBlasterGameInstance::AddPlayerStates(APlayerState* PlayerState)
{
    if (!PlayerStates.Contains(PlayerState))
    {
        PlayerStates.AddUnique(PlayerState);
    }
}

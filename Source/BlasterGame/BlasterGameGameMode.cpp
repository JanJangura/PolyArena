// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlasterGameGameMode.h"
#include "BlasterGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABlasterGameGameMode::ABlasterGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

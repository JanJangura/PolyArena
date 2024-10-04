// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// This Custom Mesh that we created called "SkeletalMesh" is not defined an enum constant by default when creating it in Unreal Engine. So we'll define it here.
// This "ECC_GameTraceChannel1" is actually the position that the SkeletalMesh has taken, so we just defined a new ECC_SkeletalMesh and linked that to ECC_GameTraceChannel1.
// Now we can use this in the Projectile Class.
#define ECC_SkeletalMesh ECollisionChannel::ECC_GameTraceChannel1
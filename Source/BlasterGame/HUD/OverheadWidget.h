// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERGAME_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget)) // This associates this C++ variable with the text block in the widget blueprint.
	class UTextBlock* DisplayText;

	// This function will be used to set the text of our DisplayText block.
	void SetDisplayText(FString TextToDisplay);

	// This is so we can call this function from anywhere, such as on our character, and pass in a pointer to the character and then we can figure out the NETROLE here.
	// We also need to pass in an APawn* Type so that way we know what to reference to obtain our role.. 
	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* InPawn);

protected:
	// This NativeDestruct()() will be called when transition to a different level or leave the current level. This allows us to remove this widget from the viewport.
	virtual void NativeDestruct() override;

};

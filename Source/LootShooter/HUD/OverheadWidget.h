// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOOTSHOOTER_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DisplayText;
	
protected:
	virtual void NativeDestruct() override;

public:
	void SetDisplayText(FString _textToDisplay);
	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* _inPawn);
	UFUNCTION(BlueprintCallable)
	void ShowPlayerName(APawn* _inPawn);
};

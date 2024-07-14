// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Menu.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup();

protected:
	virtual bool Initialize();

private:
	UPROPERTY(meta = (BindWidget))
	UButton* HostBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinBtn;

	UFUNCTION()
	void HostBtnClicked();
	UFUNCTION()
	void JoinBtnClicked();

	// The subsystem designed to handle all online session functionality
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
};

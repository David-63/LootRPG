// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LOOTSHOOTER_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* _newPlayer) override;
	virtual void Logout(AController* _exiting) override;
};

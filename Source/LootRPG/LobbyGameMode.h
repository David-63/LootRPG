// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LOOTRPG_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* _newPlayer) override;
	virtual void Logout(AController* _exiting) override;

};

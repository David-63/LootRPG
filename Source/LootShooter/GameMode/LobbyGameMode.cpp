// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* _newPlayer)
{
	Super::PostLogin(_newPlayer);

	int32 numOfPlayer = GameState.Get()->PlayerArray.Num();
	if (2 == numOfPlayer)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			bUseSeamlessTravel = true;
			world->ServerTravel(FString("/Game/Maps/OverviewMap?listen"));
		}
	}

}

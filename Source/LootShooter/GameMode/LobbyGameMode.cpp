// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* _newPlayer)
{
	Super::PostLogin(_newPlayer);

	int32 numOfPlayer = GameState.Get()->PlayerArray.Num();

	GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Yellow, FString::Printf(TEXT("Player in game : %d"), numOfPlayer));
	APlayerState* playerState = _newPlayer->GetPlayerState<APlayerState>();
	if (playerState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Cyan, FString::Printf(TEXT("%s has joined the game!"), *playerState->GetPlayerName()));
	}
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

void ALobbyGameMode::Logout(AController* _exiting)
{
	Super::Logout(_exiting);
	
	APlayerState* playerState = _exiting->GetPlayerState<APlayerState>();
	if (playerState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Cyan, FString::Printf(TEXT("%s has exited the game!"), *playerState->GetPlayerName()));
	}

	int32 numOfPlayer = GameState.Get()->PlayerArray.Num();
	GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Yellow, FString::Printf(TEXT("Player in game : %d"), numOfPlayer - 1));

}

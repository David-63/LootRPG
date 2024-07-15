// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* _newPlayer)
{
	Super::PostLogin(_newPlayer);

	if (GameState)
	{
		int32 numOfPlayers = GameState.Get()->PlayerArray.Num();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Yellow, FString::Printf(TEXT("Players in game: %d"), numOfPlayers));

			APlayerState* playerState = _newPlayer->GetPlayerState<APlayerState>();
			if (playerState)
			{
				FString playerName = playerState->GetPlayerName();
				GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Cyan, FString::Printf(TEXT("%d has joined the game"), *playerName));
			}
		}
	}
}

void ALobbyGameMode::Logout(AController* _exiting)
{
	Super::Logout(_exiting);
		
	APlayerState* playerState = _exiting->GetPlayerState<APlayerState>();
	if (playerState)
	{
		int32 numOfPlayers = GameState.Get()->PlayerArray.Num();
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Yellow, FString::Printf(TEXT("Players in game: %d"), numOfPlayers - 1));


		FString playerName = playerState->GetPlayerName();
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Cyan, FString::Printf(TEXT("%d has joined the game"), *playerName));
	}

}

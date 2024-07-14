// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
	: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete))
	, FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete))
	, JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete))
	, DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete))
	, StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))

{
	IOnlineSubsystem* subsystem = IOnlineSubsystem::Get();

	if (subsystem)
	{
		SessionInterface = subsystem->GetSessionInterface();
	}


}

void UMultiplayerSessionsSubsystem::CreateSession(int32 _numPublicConnections, FString _matchType)
{
}

void UMultiplayerSessionsSubsystem::FindSessions(int32 _maxSearchResults)
{
}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& _sessionResult)
{
}

void UMultiplayerSessionsSubsystem::DestroySession()
{
}

void UMultiplayerSessionsSubsystem::StartSession()
{
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName _sessionName, bool _successful)
{
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool _successful)
{
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName _sessionName, EOnJoinSessionCompleteResult::Type _result)
{
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName _sessionName, bool _successful)
{
}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName _sessionName, bool _successful)
{
}

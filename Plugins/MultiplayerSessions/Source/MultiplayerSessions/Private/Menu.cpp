// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

void UMenu::MenuSetup(int32 _numOfPublicConnnections, FString _typeOfMatch)
{
	NumPublicConnections = _numOfPublicConnnections;
	MatchType = _typeOfMatch;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* world = GetWorld();
	if (world)
	{
		APlayerController* playerController = world->GetFirstPlayerController();
		if (playerController)
		{
			FInputModeUIOnly inputModeData;
			inputModeData.SetWidgetToFocus(TakeWidget());
			inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			playerController->SetInputMode(inputModeData);
			playerController->SetShowMouseCursor(true);
		}
	}

	UGameInstance* gameInstance = GetGameInstance();
	if (gameInstance)
	{
		MultiplayerSessionsSubsystem = gameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}
}

bool UMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (HostBtn)
	{
		HostBtn->OnClicked.AddDynamic(this, &ThisClass::HostBtnClicked);
	}
	if (JoinBtn)
	{
		JoinBtn->OnClicked.AddDynamic(this, &ThisClass::JoinBtnClicked);
	}

	return true;
}

void UMenu::NativeDestruct()
{
	MenuTearDown();

	Super::NativeDestruct();
}

void UMenu::OnCreateSession(bool _successful)
{
	if (_successful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("Session create successfully")));

		UWorld* world = GetWorld();
		if (world)
		{
			world->ServerTravel(FString("/Game/ThirdPerson/Maps/Lobby?listen"));
		}

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Failed Session create")));
	}
}

void UMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& _sessionResult, bool _successful)
{
	if (nullptr == MultiplayerSessionsSubsystem)
	{
		return;
	}

	for (auto result : _sessionResult)
	{
		FString settingValue;
		result.Session.SessionSettings.Get(FName("MatchType"), settingValue);

		if (settingValue == MatchType)
		{
			MultiplayerSessionsSubsystem->JoinSession(result);
			return;
		}
	}
}

void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type _result)
{
	IOnlineSubsystem* subsystem = IOnlineSubsystem::Get();
	if (subsystem)
	{
		IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();
		if (sessionInterface.IsValid())
		{
			FString address;
			sessionInterface->GetResolvedConnectString(NAME_GameSession, address);

			APlayerController* playerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (playerController)
			{
				playerController->ClientTravel(address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UMenu::OnDestroySession(bool _successful)
{
}

void UMenu::OnStartSession(bool _successful)
{
}

void UMenu::HostBtnClicked()
{
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);		
	}
}

void UMenu::JoinBtnClicked()
{
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(10000);
	}
}

void UMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* world = GetWorld();
	if (world)
	{
		APlayerController* playerController = world->GetFirstPlayerController();
		if (playerController)
		{
			FInputModeGameOnly inputModeGameOnly;
			playerController->SetInputMode(inputModeGameOnly);
			playerController->SetShowMouseCursor(false);
		}
	}
}

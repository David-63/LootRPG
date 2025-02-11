// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"

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
	void MenuSetup(int32 _numOfPublicConnnections = 4, FString _typeOfMatch = FString(TEXT("FreeForAll")), FString _lobbyPath = FString(TEXT("/Game/ThirdPerson/Maps/Lobby")));

protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	//
	// Callbacks for the custom delegates on the MultiplayerSessionsSubsystem
	//
	UFUNCTION()
	void OnCreateSession(bool _successful);

	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& _sessionResult, bool _successful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type _result);

	UFUNCTION()
	void OnDestroySession(bool _successful);
	UFUNCTION()
	void OnStartSession(bool _successful);


private:
	UPROPERTY(meta = (BindWidget))
	UButton* HostBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinBtn;

	UFUNCTION()
	void HostBtnClicked();
	UFUNCTION()
	void JoinBtnClicked();

	void MenuTearDown();
	// The subsystem designed to handle all online session functionality
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	int32 NumPublicConnections{4};
	FString MatchType{TEXT("FreeForAll")};
	FString PathToLobby{TEXT("")};
};

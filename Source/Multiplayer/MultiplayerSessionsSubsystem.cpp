// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include <Online/OnlineSessionNames.h>

void PrintString(const FString& Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0, FColor::Yellow, Message);
	}
}


UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	bCreateServerAfterDestroy = false;
	DestroyServerName = "";
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//PrintString("MSS Initialize");
	IOnlineSubsystem* OnlineSubsytem = IOnlineSubsystem::Get();
	if (OnlineSubsytem)
	{
		FString SubsystemName = OnlineSubsytem->GetSubsystemName().ToString();
		PrintString(SubsystemName);

		SessionInterface = OnlineSubsytem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnFindSessionComplete);
		}
	}
}

void UMultiplayerSessionsSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("MSS Deinitialize"));
}

void UMultiplayerSessionsSubsystem::CreateServer(FString ServerName)
{
	if (ServerName.IsEmpty())
	{
		PrintString("Server name cannot be EMPTY!");
		return;
	}

	FName SessionName = FName("Co-op Adventure Session Name");

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SessionName);

	if (ExistingSession)
	{
		bCreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		SessionInterface->DestroySession(SessionName);
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;

	bool bIsLan = false;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		bIsLan = true;
	}

	SessionSettings.bIsLANMatch = bIsLan;

	SessionInterface->CreateSession(0, SessionName, SessionSettings);

}

void UMultiplayerSessionsSubsystem::FindServer(FString ServerName)
{
	PrintString("Find Session");

	if (ServerName.IsEmpty())
	{
		PrintString("Server name cannot be empty!!");
		return;
	}

	OnlineSessionSearch = MakeShareable(new FOnlineSessionSearch());

	bool bIsLan = false;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		bIsLan = true;
	}

	OnlineSessionSearch->bIsLanQuery = bIsLan;
	OnlineSessionSearch->MaxSearchResults = 999;
	OnlineSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, OnlineSessionSearch.ToSharedRef());
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		// after PATH -> ?Listen is for launch this map as a listen server
		GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?Listen");
	}
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bCreateServerAfterDestroy)
	{
		bCreateServerAfterDestroy = false;
		CreateServer(DestroyServerName);
	}
}

void UMultiplayerSessionsSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		return;
	}
	if (OnlineSessionSearch->SearchResults.Num() > 0)
	{
		PrintString(FString::Printf(TEXT("%d Session Found"), OnlineSessionSearch->SearchResults.Num()));
	}
	else
	{
		PrintString("Zero Session Found");
	}
}

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
	ServerNameToFind = "";
	MySessionName = FName("Co-op Adventure Session Name");
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
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnJoinSessionComplete);
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
		ServerCreateDelegate.Broadcast(false);
		return;
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(MySessionName);

	if (ExistingSession)
	{
		bCreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		SessionInterface->DestroySession(MySessionName);
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

	SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, MySessionName, SessionSettings);

}

void UMultiplayerSessionsSubsystem::FindServer(FString ServerName)
{
	if (ServerName.IsEmpty())
	{
		PrintString("Server name cannot be empty!!");
		JoinDelegate.Broadcast(false);
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

	ServerNameToFind = ServerName;

	SessionInterface->FindSessions(0, OnlineSessionSearch.ToSharedRef());
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	ServerCreateDelegate.Broadcast(bWasSuccessful);
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
		JoinDelegate.Broadcast(false);
		return;
	}

	if (ServerNameToFind.IsEmpty())
	{
		return;
	}

	TArray<FOnlineSessionSearchResult> Results = OnlineSessionSearch->SearchResults;
	FOnlineSessionSearchResult* CorrectResult = 0;

	if (Results.Num() > 0)
	{
		PrintString(FString::Printf(TEXT("%d Session Found"), Results.Num()));

		for (FOnlineSessionSearchResult result : Results)
		{
			if (result.IsValid())
			{
				FString ServerName = "NO-Name";
				result.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);

				if (ServerName.Equals(ServerNameToFind))
				{
					CorrectResult = &result;
					break;
				}
			}
		}

		if (CorrectResult)
		{
			SessionInterface->JoinSession(0, MySessionName, *CorrectResult);
		}
		else
		{
			PrintString("Couldn't find server");
			ServerNameToFind = "";

			JoinDelegate.Broadcast(false);
		}
	}
	else
	{
		PrintString("Zero Session Found");
		JoinDelegate.Broadcast(false);

	}
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	JoinDelegate.Broadcast(Result == EOnJoinSessionCompleteResult::Success);
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		FString SessionAddress = "";

		bool bSuccess = SessionInterface->GetResolvedConnectString(SessionName, SessionAddress);

		
		if (bSuccess)
		{
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(SessionAddress, ETravelType::TRAVEL_Absolute);
			}
		}
		else
		{
			PrintString("GetResolveConnect false");
		}

	}
	else
	{
		PrintString("OnjoinSessionComplete Failed");
	}
}

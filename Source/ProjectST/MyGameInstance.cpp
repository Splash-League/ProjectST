// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Engine/World.h"
#include "Kismet/GamePlayStatics.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"

UMyGameInstance::UMyGameInstance()
{
	MySessionName = FName("My Session");

	static ConstructorHelpers::FObjectFinder<UTexture2D> Map1Image(TEXT("/Game/FirstPersonCPP/Maps/MapImages/FirstPersonExampleMapImage"));
	if (Map1Image.Object)
	{
		FMapInfo Map;
		Map.MapName = "First Person Example Map";
		Map.MapURL = "/Game/FirstPersonCPP/Maps/FirstPersonExampleMap";
		Map.MapImage = Map1Image.Object;
		MapList.Add(Map);
		SelectedMapURL = Map.MapURL;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> Map2Image(TEXT("/Game/FirstPersonCPP/Maps/MapImages/FirstPersonExampleMapImage2"));
	if (Map2Image.Object)
	{
		FMapInfo Map;
		Map.MapName = "First Person Example Map 2";
		Map.MapURL = "/Game/FirstPersonCPP/Maps/FirstPersonExampleMap2";
		Map.MapImage = Map2Image.Object;
		MapList.Add(Map);
	}
}

void UMyGameInstance::Init()
{
	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = SubSystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			//Bind Delegates here
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMyGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnJoinSessionComplete);
		}
	}
}

void UMyGameInstance::OnCreateSessionComplete(FName SessionName, bool Succeeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, Succeeded: %d"), Succeeded);
	if (Succeeded)
	{
		GetWorld()->ServerTravel(SelectedMapURL + "?listen");
	}
}

void UMyGameInstance::OnFindSessionsComplete(bool Succeeded)
{

	SearchingForServer.Broadcast(false);
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete, Succeeded: %d"), Succeeded);

	if (Succeeded)
	{
		int32 ArrayIndex = -1;

		for (FOnlineSessionSearchResult Result : SessionSearch->SearchResults)
		{
			ArrayIndex++;
			if (!Result.IsValid())
				continue;

			FServerInfo Info;
			FString ServerName = "Empty Server Name";
			FString HostName = "Empty Host Name";

			Result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), ServerName);

			Info.ServerName = ServerName;
			Info.MaxPlayers = Result.Session.NumOpenPublicConnections;
			Info.CurrentPlayers = Info.MaxPlayers - Result.Session.NumOpenPublicConnections;
			Info.SetPlayerCount();
			Info.IsLan = Result.Session.SessionSettings.bIsLANMatch;
			Info.Ping = Result.PingInMs;
			Info.ServerArrayIndex = ArrayIndex;

			ServerListDel.Broadcast(Info);
		}
		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Server Count: %d"), SessionSearch->SearchResults.Num());
	}
}

void UMyGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete"));
	if (EOnJoinSessionCompleteResult::SessionIsFull != Result)
	{
		if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			FString JoinAddress = "";
			SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
			if (JoinAddress != "")
				PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("SERVER FULL"));

}

void UMyGameInstance::CreateServer(FCreateServerInfo ServerInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;

	//TODO: Change this to use server info for lan match
	if(IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
		SessionSettings.bIsLANMatch = false;
	else
		SessionSettings.bIsLANMatch = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = ServerInfo.MaxPlayers;

	SessionSettings.Set(FName("SERVER_NAME_KEY"), ServerInfo.ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);


	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void UMyGameInstance::FindServers()
{

	SearchingForServer.Broadcast(true);

	UE_LOG(LogTemp, Warning, TEXT("FindServers"));

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
		SessionSearch->bIsLanQuery = false;
	else
		SessionSearch->bIsLanQuery = true;

	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMyGameInstance::JoinServer(int32 ArrayIndex)
{
	FOnlineSessionSearchResult Result = SessionSearch->SearchResults[ArrayIndex];
	if (Result.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("JOINING SERVER AT INDEX: %d"), ArrayIndex);
			SessionInterface->JoinSession(0, MySessionName, Result);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("FAILED TO JOIN SERVER AT INDEX: %d"), ArrayIndex);
}

void UMyGameInstance::FillMapList()
{
	for (FMapInfo Map : MapList)
		FMapNameDel.Broadcast(Map.MapName);
}

class UTexture2D* UMyGameInstance::GetMapImage(FString MapName)
{
	for (FMapInfo Map : MapList)
	{
		if (Map.MapName.Equals(MapName))
			return Map.MapImage;
	}
	return nullptr;
}

void UMyGameInstance::SetSelectedMap(FString MapName)
{
	for (FMapInfo Map : MapList)
		if (Map.MapName.Equals(MapName))
			SelectedMapURL = Map.MapURL;
}

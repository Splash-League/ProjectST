// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MyGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FCreateServerInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FString ServerName;
	UPROPERTY(BlueprintReadWrite)
	int32 MaxPlayers;
	UPROPERTY(BlueprintReadWrite)
	bool IsLan;
};


USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
		FString ServerName;
	UPROPERTY(BlueprintReadOnly)
		FString PlayerCountStr;
	UPROPERTY(BlueprintReadOnly)
		bool IsLan;
	UPROPERTY(BlueprintReadOnly)
		int32 Ping;
	UPROPERTY(BlueprintReadOnly)
		int32 ServerArrayIndex;

	int32 CurrentPlayers;
	int32 MaxPlayers;

	void SetPlayerCount()
	{
		PlayerCountStr = FString(FString::FromInt(CurrentPlayers) + "/" + FString::FromInt(MaxPlayers));
	}
};

USTRUCT()
struct FMapInfo
{
	GENERATED_BODY()
public:
	FString MapName;
	FString MapURL;
	class UTexture2D* MapImage;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerDel, FServerInfo, ServerListDel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerSearchingDel, bool, SearchingForServer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMapInfoDel, FString, FMapNameDel);

UCLASS()
class PROJECTST_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMyGameInstance();

protected:

	FName MySessionName;
	FString SelectedMapURL;
	TArray<FMapInfo> MapList;

	UPROPERTY(BlueprintAssignable)
		FServerDel ServerListDel;

	UPROPERTY(BlueprintAssignable)
		FServerSearchingDel SearchingForServer;

	UPROPERTY(BlueprintAssignable)
		FMapInfoDel FMapNameDel;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName SessionName, bool Succeeded);
	virtual void OnFindSessionsComplete(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable)
		void CreateServer(FCreateServerInfo ServerInfo);

	UFUNCTION(BlueprintCallable)
		void FindServers();

	UFUNCTION(BlueprintCallable)
		void JoinServer(int32 ArrayIndex);

	UFUNCTION(BlueprintCallable)
		void FillMapList();

	UFUNCTION(BlueprintCallable)
		class UTexture2D* GetMapImage(FString MapName);

	UFUNCTION(BlueprintCallable)
		void SetSelectedMap(FString MapName);
};

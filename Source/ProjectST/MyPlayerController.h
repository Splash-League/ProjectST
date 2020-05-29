// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTST_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SpawnPlayer();

	UFUNCTION(Server, Reliable)
		void ServerTeamSelect(bool team);

	UFUNCTION(Server, Reliable)
		void ServerSpawn();

	UFUNCTION(BlueprintCallable)
	void teamA();

	UFUNCTION(BlueprintCallable)
	void teamB();
	
};

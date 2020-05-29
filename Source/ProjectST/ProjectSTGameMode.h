// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

//#include "MyPlayerController.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectSTGameMode.generated.h"

UCLASS(minimalapi)
class AProjectSTGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectSTGameMode();

	void PostLogin(APlayerController* NewPlayer) override;

	void TeamA(class AMyPlayerController* NewPlayer);

	void TeamB(class AMyPlayerController* NewPlayer);
	
	AActor * ChoosePlayerStart(AController * Player);

	bool ShouldSpawnAtStartSpot(AController * Player) override { return false; };
};




// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectSTGameMode.h"
#include "ProjectSTHUD.h"
#include "ProjectSTCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MyPlayerState.h"
#include "MyPlayerController.h"
#include "MyGameState.h"


AProjectSTGameMode::AProjectSTGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AProjectSTHUD::StaticClass();
}

void AProjectSTGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectSTGameMode.h"
#include "ProjectSTHUD.h"
#include "ProjectSTCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MyPlayerState.h"
#include "MyPlayerController.h"
#include "MyGameState.h"
#include "MyPlayerState.h"
#include "MyPlayerStart.h"

AProjectSTGameMode::AProjectSTGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AProjectSTHUD::StaticClass();

	PlayerStateClass = AMyPlayerState::StaticClass();
}

void AProjectSTGameMode::PostLogin(APlayerController* NewPlayer)
{
}

void AProjectSTGameMode::TeamA(AMyPlayerController* NewPlayer)
{

	//UE_LOG(LogTemp, Warning, TEXT("Successfully getting inside TeamA and PS coming back positive"));
	if (NewPlayer)
	{
		AMyPlayerState * PS = Cast<AMyPlayerState>(NewPlayer->PlayerState);
		if (PS)
		{
			UE_LOG(LogTemp, Warning, TEXT("Successfully getting inside TeamA and PS coming back positive"));
			PS->bTeamA = true;
		}
		NewPlayer->SpawnPlayer();
	}
}

void AProjectSTGameMode::TeamB(AMyPlayerController* NewPlayer)
{

	//UE_LOG(LogTemp, Warning, TEXT("Successfully getting inside TeamB and PS coming back positive"));
	if (NewPlayer)
	{
		AMyPlayerState * PS = Cast<AMyPlayerState>(NewPlayer->PlayerState);
		if (PS)
		{
			UE_LOG(LogTemp, Warning, TEXT("Successfully getting inside TeamB and PS coming back positive"));
			PS->bTeamA = false;
		}
		NewPlayer->SpawnPlayer();
	}
}

AActor * AProjectSTGameMode::ChoosePlayerStart(AController * Player)
{
	if (Player)
	{
		AMyPlayerState * PS = Cast <AMyPlayerState>(Player->PlayerState);
		if (PS)
		{
			UE_LOG(LogTemp, Warning, TEXT("Successfully getting inside chooserPlayerStart and PS coming back positive"));
			//TArray<AMyPlayerStart *> Starts;
			//for (TActorIterator<AMyPlayerState> StartItr(GetWorld()); StartItr; ++StartItr)
			//{
			//	if (StartItr->bTeamA == PS->bTeamA)
			//	{
			//		Starts.Add(*StartItr);
			//	}
			//}
			//return Starts[FMath::RandRange(0, Starts.Num() - 1)];
		}
	}
	return NULL;
}

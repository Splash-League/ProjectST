// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "ProjectSTGameMode.h"
#include "GameMode.generated.h"
#include "Kismet/GameplayStatics.h"

void AMyPlayerController::SpawnPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("Successfully getting inside SpawnPlayer"));
	if (!HasAuthority())
	{
		ServerSpawn();
	}
	else {
		AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();
		if (GameMode)
		{
			AProjectSTGameMode * PlayerGameMode = Cast<AProjectSTGameMode>(GetWorld()->GetAuthGameMode());
			APawn * NewPawn = GameMode->SpawnDefaultPawnFor(this, PlayerGameMode->ChoosePlayerStart(this));
			//APawn * NewPawn = GetWorld()->SpawnActor(AProjectSTCharacter FirstPersonCharacter, GameMode->ChoosePlayerStart(this));
			Possess(NewPawn);
		}
	}
}

void AMyPlayerController::ServerSpawn_Implementation()
{
	SpawnPlayer();
}

void AMyPlayerController::ServerTeamSelect_Implementation(bool team)
{
	if (team == true)
	{
		teamA();
	}
	else
	{
		teamB();
	}
}
void AMyPlayerController::teamA()
{
	if (!HasAuthority())
	{
		ServerTeamSelect(true);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Team A"));
		AProjectSTGameMode * GameMode = Cast<AProjectSTGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->TeamA(this);
	}

}

void AMyPlayerController::teamB()
{
	if (!HasAuthority())
	{
		ServerTeamSelect(false);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Team B"));
		AProjectSTGameMode * GameMode = Cast<AProjectSTGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->TeamB(this);
	}
}

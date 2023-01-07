// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* DedPawn)
{
	Super::PawnKilled(DedPawn);

	//UE_LOG(LogTemp, Warning, TEXT("Pawn is dead omegalul"));

	APlayerController* PlayerController = Cast<APlayerController>(DedPawn->GetController());
	
	// if the pawn's controller is a player than we know the player's lost
	if (PlayerController)
	{
		// game has ended and the player has lost
		EndGame(false);
	}
	// otherwise an ai character has died
	else
	{
		// loop all ai characters
		for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
		{
			// if any still live return - game not over
			if (!Controller->IsDead())
			{
				return;
			}
		}
		// if none still live the player has won - game over 
		EndGame(true);
	}
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	// loop through every controller
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		// if the player has won and this is the player controller, then this is the winner (and vice versa)
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;

		// end the game
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}

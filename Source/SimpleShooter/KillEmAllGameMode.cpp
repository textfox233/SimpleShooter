// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"

void AKillEmAllGameMode::PawnKilled(APawn* DedPawn)
{
	Super::PawnKilled(DedPawn);

	//UE_LOG(LogTemp, Warning, TEXT("Pawn is dead omegalul"));

	APlayerController* PlayerController = Cast<APlayerController>(DedPawn->GetController());
	
	// if the pawn's controller is a player than we know the player's lost
	if (PlayerController)
	{
		// game has ended and this pawn's owner is the loser
		PlayerController->GameHasEnded(nullptr, false);
	}
}

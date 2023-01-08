// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	HUD->RemoveFromViewport();

	if (bIsWinner) // the player has won
	{
		// Load win screen widget
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);

		// null check
		if (WinScreen)
		{
			// add widget to the viewport
			WinScreen->AddToViewport();
		}
	}
	else // the player has lost
	{
		// Load lose screen widget
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);

		// null check
		if (LoseScreen)
		{
			// add widget to the viewport
			LoseScreen->AddToViewport();
		}
	}

	// Start timer for restart
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

void AShooterPlayerController::BeginPlay() 
{
	Super::BeginPlay();

	// Load HUD widget
	HUD = CreateWidget(this, HUDClass);

	// null check
	if (HUD)
	{
		// add widget to the viewport
		HUD->AddToViewport();
	}
}

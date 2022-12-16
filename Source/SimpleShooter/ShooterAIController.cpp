// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior);

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
	}
}

void AShooterAIController::Tick(float DeltaTime) // Called every frame
{
	Super::Tick(DeltaTime);

	//// pointer to the player pawn
	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//// follow at the player
	//MoveToActor(PlayerPawn, AcceptanceRadius, false);
	//// If LineOfSight
	//if (LineOfSightTo(PlayerPawn))
	//{
	//	// Move To
	//	MoveToActor(PlayerPawn, 200.f, false);
	//	// Set Focus
	//	SetFocus(PlayerPawn);
	//}
	//// Else
	//else 
	//{
	//	// Clear Focus
	//	ClearFocus(EAIFocusPriority::LastFocusPriority);
	//	// Stop Movement
	//	StopMovement();
	//}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"


void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior);

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		//  set blackboard values
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), this->GetPawn()->GetActorLocation());
	}
}

void AShooterAIController::Tick(float DeltaTime) // Called every frame
{
	Super::Tick(DeltaTime);
}

bool AShooterAIController::IsDead() const // used to determine player win condition
{
	// find the pawn for this character
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());

	if (ControlledCharacter) // null check
	{
		// pass on to evaluate if dead
		return ControlledCharacter->IsDead();
	}
	return true; // mark as dead if pawn can't be found
}
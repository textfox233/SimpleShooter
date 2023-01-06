// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// call super
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// if owner's null return failure
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// otherwise get the character
	AShooterCharacter* Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	// if character's null return failure
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// otherwise shoot the gun
	Character->Shoot();

	// return success
	return EBTNodeResult::Succeeded;
}

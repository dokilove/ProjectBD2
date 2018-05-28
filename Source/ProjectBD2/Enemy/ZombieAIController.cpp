// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAIController.h"
#include "Enemy/MyZombie.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/MyZombie.h"
#include "BehaviorTree/BehaviorTree.h"


AZombieAIController::AZombieAIController()
{
	BT_Zombie = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	BB_Zombie = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}


void AZombieAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AMyZombie* Pawn = Cast<AMyZombie>(InPawn);
	if (Pawn)
	{
		BB_Zombie->InitializeBlackboard(*(Pawn->BehaviorTree->BlackboardAsset));
		BT_Zombie->StartTree(*(Pawn->BehaviorTree), EBTExecutionMode::Looped);
	}
}
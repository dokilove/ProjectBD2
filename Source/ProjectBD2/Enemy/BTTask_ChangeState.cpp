// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ChangeState.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/MyZombie.h"

EBTNodeResult::Type UBTTask_ChangeState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMyZombie* Zombie = Cast<AMyZombie>(OwnerComp.GetAIOwner()->GetPawn());

	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		Zombie->CurrentState = NewZombieState;
		Zombie->CurrentAnimState = NewZombieAnimState;

		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), (uint8)Zombie->CurrentState);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

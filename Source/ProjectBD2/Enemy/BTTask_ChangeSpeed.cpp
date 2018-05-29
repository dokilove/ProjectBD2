// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ChangeSpeed.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Enemy/MyZombie.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTask_ChangeSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyZombie* Zombie = Cast<AMyZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		switch (Zombie->CurrentAnimState)
		{
		case EZombieAnimState::Walk:
			Zombie->GetCharacterMovement()->MaxWalkSpeed = Zombie->WalkSpeed;
			break;
		case EZombieAnimState::Run:
			Zombie->GetCharacterMovement()->MaxWalkSpeed = Zombie->RunSpeed;
			break;
		}

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
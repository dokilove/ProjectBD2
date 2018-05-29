// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckState.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/MyZombie.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"

void UBTService_CheckState::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AMyZombie* Zombie = Cast<AMyZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		switch (Zombie->CurrentState)
		{
			case EZombieState::Chase:
			{
				AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
				if (Player && Player->IsValidLowLevelFast())
				{
					float Range = FVector::Distance(Zombie->GetActorLocation(), Player->GetActorLocation());

					if (Range <= Zombie->AttackRange)
					{
						Zombie->CurrentState = EZombieState::Battle;
						OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);
					}
					else if (Range > Zombie->PawnSensing->SightRadius)
					{
						Zombie->CurrentState = EZombieState::Normal;
						OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);
					}
				}
			}
			break;
			case EZombieState::Battle:
			{
				AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
				if (Player && Player->IsValidLowLevelFast())
				{
					float Range = FVector::Distance(Zombie->GetActorLocation(), Player->GetActorLocation());
					
					if (Range > Zombie->PawnSensing->SightRadius)
					{
						Zombie->CurrentState = EZombieState::Normal;
						OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);
					}
				}
			}
			break;
		}
	}
}

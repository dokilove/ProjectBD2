// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CheckChase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/MyZombie.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "Player/MyCharacter.h"
#include "Kismet/KismetSystemLibrary.h"


EBTNodeResult::Type UBTTask_CheckChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyZombie* Zombie = Cast<AMyZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
		if (Player && Player->IsValidLowLevelFast())
		{
			float Range = FVector::Distance(Zombie->GetActorLocation(), Player->GetActorLocation());

			if (Range <= Zombie->AttackRange)
			{
				Zombie->CurrentState = EZombieState::Battle;
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);
				return EBTNodeResult::Failed;
			}
			else if (Range > Zombie->PawnSensing->SightRadius)
			{
				Zombie->CurrentState = EZombieState::Normal;
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);
				return EBTNodeResult::Failed;
			}
			else
			{
				FVector TraceStart = Zombie->GetActorLocation();
				FVector TraceEnd = Player->GetActorLocation();
				TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
				TArray<AActor*> ActorsToIgnore;
				FHitResult OutHit;

				ObjectTypes
					.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
				ObjectTypes
					.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
				ObjectTypes
					.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
				ActorsToIgnore.Add(Zombie);

				bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Red, FLinearColor::Yellow, 5.0f);

				if (Result)
				{
					if (!OutHit.GetActor()->ActorHasTag(FName(TEXT("Player"))))
					{
						Zombie->CurrentState = EZombieState::Normal;
						OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);
						return EBTNodeResult::Failed;
					}
				}
			}
		}
	}
	
	return EBTNodeResult::Succeeded;
}


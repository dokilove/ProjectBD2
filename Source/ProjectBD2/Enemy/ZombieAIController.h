// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD2_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AZombieAIController();
	
	virtual void Possess(APawn* InPawn) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBehaviorTreeComponent* BT_Zombie;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBlackboardComponent* BB_Zombie;
};

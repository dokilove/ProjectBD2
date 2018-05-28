// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy/MyZombie.h"
#include "MyZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD2_API UMyZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	
public:
	UMyZombieAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		EZombieState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		EZombieAnimState CurrentAnimState;
};

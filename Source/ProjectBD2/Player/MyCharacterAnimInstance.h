// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD2_API UMyCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation_WorkerThread(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Info")
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Info")
		float Direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info")
		bool bIsCrouch;
	
};

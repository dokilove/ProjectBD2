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


	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info")
		float Yaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info")
		float Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info")
		bool bIsIronsight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info")
		bool bIsSprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info")
		bool bIsProne;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info")
		bool bIsMovingLocked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info")
		bool bIsFire;
	UFUNCTION()
		void AnimNotify_LockMove();
	UFUNCTION()
		void AnimNotify_UnlockMove();
};

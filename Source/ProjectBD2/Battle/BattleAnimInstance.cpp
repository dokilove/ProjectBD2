// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleAnimInstance.h"
#include "Battle/BattleCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBattleAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ABattleCharacter* Pawn = Cast<ABattleCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Speed = Pawn->GetCharacterMovement()->Velocity.Size();
		Direction = CalculateDirection(Pawn->GetCharacterMovement()->Velocity, Pawn->GetActorRotation());
		bIsCrouch = Pawn->bIsCrouched;
		Yaw = Pawn->GetAimoffset().Yaw;
		Pitch = Pawn->GetAimoffset().Pitch;
		bIsIronsight = Pawn->bIsIronsight;
		bIsSprint = Pawn->bIsSprint;
		bIsProne = Pawn->bIsProne;
		bIsMovingLocked = Pawn->bIsMovingLocked;
		bIsFire = Pawn->bIsFire;
	}
}

void UBattleAnimInstance::AnimNotify_LockMove()
{
	ABattleCharacter* Pawn = Cast<ABattleCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Pawn->bIsMovingLocked = true;
	}
}

void UBattleAnimInstance::AnimNotify_UnlockMove()
{
	ABattleCharacter* Pawn = Cast<ABattleCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Pawn->bIsMovingLocked = false;
	}
}

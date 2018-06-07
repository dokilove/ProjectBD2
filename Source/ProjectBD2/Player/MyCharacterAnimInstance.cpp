// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacterAnimInstance.h"
#include "Player/MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMyCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMyCharacter* Pawn = Cast<AMyCharacter>(TryGetPawnOwner());
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

void UMyCharacterAnimInstance::AnimNotify_LockMove()
{
	AMyCharacter* Pawn = Cast<AMyCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Pawn->bIsMovingLocked = true;
	}
}

void UMyCharacterAnimInstance::AnimNotify_UnlockMove()
{
	AMyCharacter* Pawn = Cast<AMyCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Pawn->bIsMovingLocked = false;
	}
}

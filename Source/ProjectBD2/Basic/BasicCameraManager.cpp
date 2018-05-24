// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicCameraManager.h"
#include "Player/MyCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"


ABasicCameraManager::ABasicCameraManager()
{

}

void ABasicCameraManager::BeginPlay()
{
	Super::BeginPlay();	
}

void ABasicCameraManager::UpdateCamera(float DeltaTime)
{
	FVector TargetOffset;
	Super::UpdateCamera(DeltaTime); 
	
	AMyCharacter* Pawn = PCOwner ? Cast<AMyCharacter>(PCOwner->GetPawn()) : nullptr;

	if (Pawn)
	{

		if (Pawn->bIsProne)
		{
			TargetOffset = Pawn->ProneSpringArmPosition;
		}
		else if (Pawn->bIsCrouched)
		{
			TargetOffset = Pawn->CrouchSpringArmPosition;
		}
		else
		{
			TargetOffset = Pawn->NormalSpringArmPosition;
		}
		


		SpringArmOffset = FMath::VInterpTo(SpringArmOffset, TargetOffset, DeltaTime, 5.0f);

		Pawn->SpringArm->SetRelativeLocation(SpringArmOffset);

		//UE_LOG(LogClass, Warning, TEXT("DefaultCameraZ %f"), DefaultCameraZ);
		//UE_LOG(LogClass, Warning, TEXT("Crouched offst %f"), CrouchOffset);

	}
}

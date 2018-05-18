// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicCameraManager.h"
#include "Player/MyCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"


ABasicCameraManager::ABasicCameraManager()
{

}

void ABasicCameraManager::BeginPlay()
{
	Super::BeginPlay();
	AMyCharacter* Pawn = PCOwner ? Cast<AMyCharacter>(PCOwner->GetPawn()) : nullptr;

	if (Pawn)
	{
		DefaultCameraZ = Pawn->Camera->GetRelativeTransform().GetLocation().Z;
	}
}

void ABasicCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime); AMyCharacter* Pawn = PCOwner ? Cast<AMyCharacter>(PCOwner->GetPawn()) : nullptr;

	if (Pawn)
	{
		float TargetOffset = Pawn->bIsCrouched ? Pawn->CrouchedEyeHeight : 0;

		CrouchOffset = FMath::FInterpTo(CrouchOffset, TargetOffset, DeltaTime, 5.0f);

		FVector CurrentCameraLocation = Pawn->Camera->GetRelativeTransform().GetLocation();
		FVector NewCameraLocation = FVector(CurrentCameraLocation.X, CurrentCameraLocation.Y,
			DefaultCameraZ - CrouchOffset);
		//UE_LOG(LogClass, Warning, TEXT("DefaultCameraZ %f"), DefaultCameraZ);
		//UE_LOG(LogClass, Warning, TEXT("Crouched offst %f"), CrouchOffset);

		Pawn->Camera->SetRelativeLocation(NewCameraLocation);
	}
}

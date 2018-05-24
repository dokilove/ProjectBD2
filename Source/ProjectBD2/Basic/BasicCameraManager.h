// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BasicCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD2_API ABasicCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:

	ABasicCameraManager();
	virtual void BeginPlay() override;
	virtual void UpdateCamera(float DeltaTime) override; 
	
	FVector SpringArmOffset;
};

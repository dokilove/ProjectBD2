// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Item/ItemDataTableComponent.h"
#include "MasterItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD2_API AMasterItem : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	AMasterItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UItemDataTableComponent* ItemDataTable;
	
	
};

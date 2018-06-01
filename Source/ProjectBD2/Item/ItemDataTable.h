// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataTable.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	UnKnown =0 UMETA(DisplayName="UnKnown"),
	Equip = 1 UMETA(DisplayName = "Equip"),
	Consume = 2 UMETA(DisplayName = "Consume"),
}; 

UENUM(BlueprintType)
enum class EEquipSlot : uint8
{
	NotUse = 0 UMETA(DisplayName = "NotUse"),
	WeaponRightHand = 1 UMETA(DisplayName = "WeaponRightHand"),
	Weapon = 2 UMETA(DisplayName = "Weapon"),
	WeaponBack01 = 3 UMETA(DisplayName = "WeaponBack01"),
};
/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTBD2_API FItemDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	//RowName	ItemIndex	ItemName	ItemType	ItemEquipSlot	ItemFloat01	ItemCount	ItemThumnail	ItemMesh
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int ItemIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		EEquipSlot ItemEquipSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float ItemFloat01;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int ItemCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TAssetPtr<class UTexture2D> ItemThumnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TAssetPtr<class UStaticMesh> ItemMesh;
};

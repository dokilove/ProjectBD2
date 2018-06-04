// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemDataTable.h"
#include "ItemSlotWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD2_API UItemSlotWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	// UI 마우스 입력 추가시 build.cs 에 UMG 모듈추가하고 Slate사용 관련으로 주석풀어주어야 할ㄴ다
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UImage* ItemThumb;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UTextBlock* ItemName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UTextBlock* ItemCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UButton* ItemButton;

	class UDataTable* DataTable;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FItemDataTable Data;

	bool bIsEmpty = true;

	UFUNCTION(BlueprintCallable)
		void SetItem(int NewItemIndex);
	UFUNCTION(BlueprintCallable)
		void UseItem();
	UFUNCTION(BlueprintCallable)
		void ClearItem();
	UFUNCTION(BlueprintCallable)
		void AddItemCount(int AddItemCount);
	UFUNCTION(BlueprintCallable)
		bool IsConsume();
	UFUNCTION(BlueprintCallable)
		bool GetItemTableRow(FItemDataTable& ItemData);
};

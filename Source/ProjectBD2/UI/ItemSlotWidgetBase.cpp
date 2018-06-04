// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemSlotWidgetBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Engine/StreamableManager.h"

void UItemSlotWidgetBase::NativeConstruct()
{
	// 시작시에 일단 표시하지 않음
	SetVisibility(ESlateVisibility::Collapsed);

	ItemThumb = Cast<UImage>(GetWidgetFromName(TEXT("ItemThumb")));
	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));
	ItemButton = Cast<UButton>(GetWidgetFromName(TEXT("ItemButton")));
	ItemButton->OnClicked.AddDynamic(this, &UItemSlotWidgetBase::UseItem);
}

FReply UItemSlotWidgetBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Unhandled();
}

void UItemSlotWidgetBase::SetItem(int NewItemIndex)
{
}

void UItemSlotWidgetBase::UseItem()
{
}

void UItemSlotWidgetBase::ClearItem()
{
}

void UItemSlotWidgetBase::AddItemCount(int AddItemCount)
{
}

bool UItemSlotWidgetBase::IsConsume()
{
	return false;
}

bool UItemSlotWidgetBase::GetItemTableRow(FItemDataTable & ItemTable)
{
	return false;
}

﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemSlotWidgetBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Engine/StreamableManager.h"
#include "Player/MyCharacter.h"
#include "Kismet/GameplayStatics.h"

void UItemSlotWidgetBase::NativeConstruct()
{
	// 시작시에 일단 표시하지 않음
	SetVisibility(ESlateVisibility::Collapsed);
	
	ItemThumb = Cast<UImage>(GetWidgetFromName(TEXT("ItemThumb")));
	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));
	ItemButton = Cast<UButton>(GetWidgetFromName(TEXT("ItemButton")));
	ItemButton->OnClicked.AddDynamic(this, &UItemSlotWidgetBase::UseItem);

	FStringAssetReference DataTableRef(TEXT("DataTable'/Game/Blueprints/Data/ItemTable.ItemTable'"));

	FStreamableManager AssetLoader;
	DataTable = AssetLoader.LoadSynchronous<UDataTable>(DataTableRef);
}

FReply UItemSlotWidgetBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton)
		&& !bIsEmpty)
	{
		UseItem();
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void UItemSlotWidgetBase::SetItem(int NewItemIndex)
{
	Data.ItemIndex = NewItemIndex;
	if (GetItemTableRow(Data))
	{
		if (ItemThumb)
		{
			FStreamableManager AssetLoader;
			UTexture2D* ItemTexture = AssetLoader.LoadSynchronous<UTexture2D>(Data.ItemThumnail);
			ItemThumb->SetBrushFromTexture(ItemTexture);
			
		}
		if (ItemName)
		{
			ItemName->SetText(FText::FromString(Data.ItemName));
		}
		if (ItemCount)
		{
			ItemCount->SetText(FText::FromString(FString::FromInt(Data.ItemCount)));
		}
		bIsEmpty = false;
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UItemSlotWidgetBase::UseItem()
{
	if (IsConsume())
	{
		AMyCharacter* Pawn = Cast<AMyCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
		if (Pawn)
		{
			Pawn->CurrentHP += Data.ItemFloat01;
			Pawn->CurrentHP = FMath::Clamp<float>(Pawn->CurrentHP, 0, Pawn->MaxHP);
			Data.ItemCount--;


			if (Data.ItemCount == 0)
			{
				ClearItem();
			}
			else
			{
				if (ItemCount)
				{
					ItemCount->SetText(FText::FromString(FString::FromInt(Data.ItemCount)));
				}
			}
		}
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("Equip"));
	}

}

void UItemSlotWidgetBase::ClearItem()
{
	bIsEmpty = true;
	SetVisibility(ESlateVisibility::Collapsed);
}

void UItemSlotWidgetBase::AddItemCount(int AddItemCount)
{
	Data.ItemCount += AddItemCount;
	if (ItemCount)
	{
		ItemCount->SetText(FText::FromString(FString::FromInt(Data.ItemCount)));
	}
}

bool UItemSlotWidgetBase::IsConsume()
{
	return Data.ItemType == EItemType::Consume;
}

bool UItemSlotWidgetBase::GetItemTableRow(FItemDataTable & ItemData)
{
	if (DataTable)
	{
		if (ItemData.ItemIndex != 0)
		{
			ItemData = *DataTable->FindRow<FItemDataTable>(FName(*FString::FromInt(ItemData.ItemIndex)), TEXT("ItemIndex"), true);
		}

		if (nullptr == &ItemData)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}

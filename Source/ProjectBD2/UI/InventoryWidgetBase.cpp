// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryWidgetBase.h"
#include "Components/ScrollBox.h"
#include "UI/ItemSlotWidgetBase.h"
#include "Battle/BattlePC.h"
#include "Kismet/GameplayStatics.h"

void UInventoryWidgetBase::NativeConstruct()
{
	InventoryScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("InventoryScrollBox")));

	if (InventoryScrollBox)
	{
		//for (int i = 0; i < InventoryScrollBox->GetChildrenCount(); ++i)
		//{
		//	UItemSlotWidgetBase* Slot = Cast<UItemSlotWidgetBase>(InventoryScrollBox->GetChildAt(i));
		//	if (Slot)
		//	{
		//		Slots.Add(Slot);
		//	}
		//}

		// 슬롯 아이템 동적 생성
		ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		FStringClassReference SlotRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/ItemSlotWidget.ItemSlotWidget_C'"));
		
		if (UClass* SlotClass = SlotRef.TryLoadClass<UUserWidget>())
		{
			for (int i = 0; i < 12; ++i)
			{
				UItemSlotWidgetBase* Slot = CreateWidget<UItemSlotWidgetBase>(PC, SlotClass);
				InventoryScrollBox->AddChild(Slot);
				Slots.Add(Slot);
			}
		}
	}
}

UItemSlotWidgetBase * UInventoryWidgetBase::GetEmptySlot()
{
	for (auto Slot : Slots)
	{
		if (Slot->bIsEmpty)
		{
			return Slot;
		}
	}

	return nullptr;
}

UItemSlotWidgetBase * UInventoryWidgetBase::GetSameIDSlot(int ItemIndex)
{
	for (auto Slot : Slots)
	{
		if (Slot->Data.ItemIndex == ItemIndex
			&& !Slot->bIsEmpty
			&& Slot->IsConsume())
		{
			return Slot;
		}
	}

	return nullptr;
}

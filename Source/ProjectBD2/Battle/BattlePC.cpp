// Fill out your copyright notice in the Description page of Project Settings.

#include "BattlePC.h"
#include "Battle/BattlePCM.h"
#include "UI/ItemTooltipWidgetBase.h"
#include "UI/InventoryWidgetBase.h"
#include "Item/MasterItem.h"

ABattlePC::ABattlePC()
{
	PlayerCameraManagerClass = ABattlePCM::StaticClass();
}

void ABattlePC::BeginPlay()
{
	S2C_SetupWidget();
}

void ABattlePC::S2C_SetupWidget_Implementation()
{
	FStringClassReference ItemTooltipRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/ItemTooltipWidget.ItemTooltipWidget_C'"));
	if (UClass* ItemTooltipClass = ItemTooltipRef.TryLoadClass<UUserWidget>())
	{
		ItemTooltip = Cast<UItemTooltipWidgetBase>(CreateWidget<UUserWidget>(this, ItemTooltipClass));
		ItemTooltip->AddToViewport();
		ItemTooltip->SetVisibility(ESlateVisibility::Collapsed);
	}

	FStringClassReference InventoryRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/InventoryWidget.InventoryWidget_C'"));
	if (UClass* InventoryClass = InventoryRef.TryLoadClass<UUserWidget >())
	{
		Inventory = Cast<UInventoryWidgetBase>(CreateWidget<UUserWidget>(this, InventoryClass));
		Inventory->AddToViewport();
		Inventory->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ABattlePC::ToggleInventory()
{
	if (Inventory->GetVisibility() == ESlateVisibility::Collapsed)
	{
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
		Inventory->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		Inventory->SetVisibility(ESlateVisibility::Collapsed);
	}
}


bool ABattlePC::C2S_DestroyItem_Validate(class AMasterItem* Item)
{
	return true;
}

void ABattlePC::C2S_DestroyItem_Implementation(class AMasterItem* Item)
{
	Item->Destroy();
}

void ABattlePC::S2C_BloodEffect_Implementation()
{
	FStringClassReference BloodEffectWidgetRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BloodEffectWidget.BloodEffectWidget_C'"));
	if (UClass* BloodEffectWidgetClass = BloodEffectWidgetRef.TryLoadClass<UUserWidget>())
	{
		UUserWidget* BloodEffectWidget = CreateWidget<UUserWidget>(this, BloodEffectWidgetClass);
		BloodEffectWidget->AddToViewport();
	}
}

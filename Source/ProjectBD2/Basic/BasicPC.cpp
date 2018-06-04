// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicPC.h"
#include "Basic/BasicCameraManager.h"
#include "UI/ItemTooltipWidgetBase.h"
#include "UI/InventoryWidgetBase.h"

ABasicPC::ABasicPC()
{
	PlayerCameraManagerClass = ABasicCameraManager::StaticClass();
}

void ABasicPC::BeginPlay()
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

void ABasicPC::ToggleInventory()
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

// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicPC.h"
#include "Basic/BasicCameraManager.h"
#include "UI/ItemTooltipWidgetBase.h"

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
}

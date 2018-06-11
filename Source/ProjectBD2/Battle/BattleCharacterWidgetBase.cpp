// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleCharacterWidgetBase.h"
#include "Battle/BattleCharacter.h"
#include "Components/ProgressBar.h"


void UBattleCharacterWidgetBase::NativeConstruct()
{
	HPBar = Cast<UProgressBar>(GetWidgetFromName("HPBar"));
}

void UBattleCharacterWidgetBase::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	ABattleCharacter* Player = Cast<ABattleCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		SetHPBar(Player->CurrentHP / Player->MaxHP);
	}
}

void UBattleCharacterWidgetBase::SetHPBar(float Ratio)
{
	if (HPBar)
	{
		HPBar->SetPercent(Ratio);
	}
}

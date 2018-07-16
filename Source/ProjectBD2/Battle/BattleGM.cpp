// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleGM.h"
#include "Battle/ItemPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Item/MasterItem.h"

void ABattleGM::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemPoint::StaticClass(), ItemPoints);

	FStringClassReference MasterItemRef(TEXT("Blueprint'/Game/Blueprints/Item/BP_MasterItem.BP_MasterItem_C'"));
	if (UClass* MasterItemClass = MasterItemRef.TryLoadClass<AMasterItem>())
	{
		for (auto ItemPoint : ItemPoints)
		{
			GetWorld()->SpawnActor<AMasterItem>(MasterItemClass, ItemPoint->GetActorLocation(), ItemPoint->GetActorRotation());
		}
	}
}

void ABattleGM::Do0()
{
	GLog->Log(TEXT("ÅÊÄ¿°¡ µµ¸Á°¨"));
}

void ABattleGM::Do1(int Number)
{
	GLog->Log(FString::Printf(TEXT("ÅÊÄ¿°¡ µµ¸Á°¨ %dÃÊ ÁØ´Ù. ¾î¿© ¿Í¶ó"), Number));
}

void ABattleGM::Do2(int Number, float Number2)
{
	GLog->Log(FString::Printf(TEXT("%d %f"), Number, Number2));
}

void ABattleGM::Do3(int Number, float Number2, FString String)
{
	GLog->Log(FString::Printf(TEXT("%d %f %s"), Number, Number2, *String));
}



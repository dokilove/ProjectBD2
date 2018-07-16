// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleGM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD2_API ABattleGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	TArray <class AActor*> ItemPoints;
	
	// 콘솔 함수
	UFUNCTION(Exec)
		void Do0();
	UFUNCTION(Exec)
		void Do1(int Number);
	UFUNCTION(Exec)
		void Do2(int Number, float Number2);
	UFUNCTION(Exec)
		void Do3(int Number, float Number2, FString String);

};

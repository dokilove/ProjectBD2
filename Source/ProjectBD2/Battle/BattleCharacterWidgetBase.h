// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleCharacterWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD2_API UBattleCharacterWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UProgressBar* HPBar;

	void SetHPBar(float Ratio);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD2_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	
	
public:
	AMyHUD();
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* Crosshair;


	// 레이더
	void DrawRadarBG();
	void DrawPlayer();
	void FindRadarObject();
	void DrawRadarObject();

	// 레이더 오브젝트 사이즈
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DrawRadarObjectSize = 5;
	// 화면상 레이더 위치,
	// 왼쪽위 -> 오른쪽 아래로 좌표 증가
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D RadarPosition = FVector2D(0.8f, 0.8f);
	// 레이더 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RadarRadius = 100.0f;
	// 레이더 가운데 좌표
	FVector2D GetRadarCenterPosition();
	// 레이더에 그릴 물체 리스트
	TArray<class AActor*> RadarObjectList;
	// 레이더 그릴 물체 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString RadarObjectTagName;
	// 레이더 축소 비율
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RadarRatio = 10.0f;

	// 주인공 기준 로컬 좌표계로 변환, 레이더 축소 비율까지 적용
	FVector2D ConvertWorldToPlayerLocal(class AActor* RadarActor);
};

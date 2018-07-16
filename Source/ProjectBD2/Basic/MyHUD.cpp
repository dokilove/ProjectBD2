// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"
#include "Item/MasterItem.h"

AMyHUD::AMyHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Crosshair(TEXT("Texture2D'/Game/TPSData/Weapons/crosshair.crosshair'"));
	if (T_Crosshair.Succeeded())
	{
		Crosshair = T_Crosshair.Object;
	}
}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();
	if (Crosshair)
	{
		DrawTextureSimple(Crosshair,
			Canvas->SizeX/2 - Crosshair->GetSizeX() / 2,
			Canvas->SizeY/2 - Crosshair->GetSizeY() / 2);

	}

	// Draw RadarBG()
	DrawRadarBG();
	// Draw Player()
	DrawPlayer();
	// Find Radar Object()
	FindRadarObject();
	// Draw Radat Object()
	DrawRadarObject();

}

void AMyHUD::DrawRadarBG()
{
	FVector2D RadarCenter = GetRadarCenterPosition();

	for (int i =0; i < 360; ++i)
	{ 	
		float X = FMath::Cos(i) * RadarRadius;
		float Y = FMath::Sin(i) * RadarRadius;

		DrawLine(RadarCenter.X, RadarCenter.Y, RadarCenter.X + X, RadarCenter.Y + Y, FLinearColor::Gray, 3.0f);
	}

}

void AMyHUD::DrawPlayer()
{
	FVector2D RadarCenter = GetRadarCenterPosition();

	DrawRect(FLinearColor::Red, RadarCenter.X, RadarCenter.Y, DrawRadarObjectSize, DrawRadarObjectSize);
}

void AMyHUD::FindRadarObject()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMasterItem::StaticClass(), RadarObjectList);

	// 태그를 쓰면
	//UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(*RadarObjectTagName), RadarObjectList);
}

void AMyHUD::DrawRadarObject()
{
	APawn* MyPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	FVector2D RadarCenter = GetRadarCenterPosition();

	for (auto It : RadarObjectList)
	{
		FVector2D ConvertLocation = ConvertWorldToPlayerLocal(It);

		FVector Temp = FVector(ConvertLocation.X, ConvertLocation.Y, 0);
		// 레이더 바깥에 있는 오브젝트는 가장자리에 걸리게 그려준다
		Temp = Temp.GetClampedToMaxSize2D(RadarRadius - DrawRadarObjectSize);

		DrawRect(FLinearColor::Blue, RadarCenter.X + Temp.X, RadarCenter.Y + Temp.Y, DrawRadarObjectSize, DrawRadarObjectSize);
	}
}

FVector2D AMyHUD::GetRadarCenterPosition()
{
	if (Canvas)
	{
		return FVector2D(Canvas->SizeX * RadarPosition.X, Canvas->SizeY * RadarPosition.Y);
	}

	return FVector2D();
}

FVector2D AMyHUD::ConvertWorldToPlayerLocal(AActor * RadarActor)
{
	APawn* MyPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (MyPlayer && RadarActor)
	{
		// MyPlayer 기준으로 로컬 좌표를 가져오는 함수
		FVector ActorLocal3DPosition = MyPlayer->GetTransform().InverseTransformPosition(RadarActor->GetActorLocation());

		// 좌우를 바꿔준다
		ActorLocal3DPosition = FRotator(0, -90, 0).RotateVector(ActorLocal3DPosition);

		// 지금 거리가 실거리로 되어있어서 비율에 맞춰서 줄여준다
		ActorLocal3DPosition /= RadarRatio;

		// 높낮이는 안쓰니 Z축 비운다
		return FVector2D(ActorLocal3DPosition);
	}

	return FVector2D();
}

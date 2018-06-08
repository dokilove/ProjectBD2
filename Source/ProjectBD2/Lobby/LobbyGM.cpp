// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGM.h"
#include "TimerManager.h"
#include "Lobby/LobbyGS.h"
#include "Engine/World.h"

void ALobbyGM::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(StartGameTimeHandle, this, &ALobbyGM::StartGameTimer, 1.0f, false);

}

void ALobbyGM::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		// 1. 플레이어 컨트롤러가 생성될때마다기록
		//GS->UserCount++;

		// 2. 현재 플레이어 개수 세기
		//GS->UserCount = 0;
		//for (auto i = GetWorld()->GetPlayerControllerIterator(); i; ++i)
		//{
		//	GS->UserCount++;
		//}
		

		// 3.PlayerState개수 세기
		//GS->UserCount = GS->PlayerArray.Num();

		GS->UserCount = GetNumPlayers();

		if (HasAuthority())
		{
			GS->OnRep_UserCount();
		}
	}
}

void ALobbyGM::StartGameTimer()
{
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->LeftTime--;
		if (HasAuthority())
		{
			GS->OnRep_LeftTime();
		}
		//UE_LOG(LogClass, Warning, TEXT("StartGameTimer %d"), GS->LeftTime);
		if (GS->LeftTime > 0)
		{
			GetWorldTimerManager().SetTimer(StartGameTimeHandle, this, &ALobbyGM::StartGameTimer, 1.0f, false);
		}
	}
}

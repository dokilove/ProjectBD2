// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGS.h"
#include "UnrealNetwork.h"
#include "Lobby/LobbyPC.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void ALobbyGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGS, LeftTime);
	DOREPLIFETIME(ALobbyGS, UserCount);
}

void ALobbyGS::OnRep_LeftTime()
{

	//UE_LOG(LogClass, Warning, TEXT("Left Time : %d"), LeftTime);

	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		//UE_LOG(LogClass, Warning, TEXT("OnRep_LeftTime %d"), LeftTime);
		if (LeftTime <= 0)
		{
			if (HasAuthority())
			{
				PC->StartGame();
			}
		}
		else
		{
			PC->SetPublicMessage(LeftTime);
		}
	}
}

void ALobbyGS::OnRep_UserCount()
{
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->SetAlive(UserCount);
	}
}

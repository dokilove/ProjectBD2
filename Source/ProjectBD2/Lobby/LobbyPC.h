// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD2_API ALobbyPC : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	virtual void BeginPlay() override;
	
	UFUNCTION(Client, Reliable)
	void S2C_SetupWidget();
	void S2C_SetupWidget_Implementation();
	void StartGame();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ULobbyWidgetBase* LobbyWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UUserWidget* LoadingWidget;

	UFUNCTION(Client, Reliable)
	void S2C_ViewLoading();
	void S2C_ViewLoading_Implementation();

	FTimerHandle StartGameTimer;
	UFUNCTION()
	void GameStartTimer();
};

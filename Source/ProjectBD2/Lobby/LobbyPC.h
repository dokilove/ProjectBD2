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

	UFUNCTION(Server, Reliable, WithValidation)
		void C2S_SendChatMessage(const FText& Message);
	bool C2S_SendChatMessage_Validate(const FText& Message);
	void C2S_SendChatMessage_Implementation(const FText& Message);

	UFUNCTION(Client, Reliable)
		void S2C_AddChatMessage(const FText& Message);
		void S2C_AddChatMessage_Implementation(const FText& Message);

		void SetPublicMessage(int LeftTime);
		void SetAlive(int UserCount);
};

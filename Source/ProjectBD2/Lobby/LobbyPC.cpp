// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPC.h"
#include "UI/LobbyWidgetBase.h"
#include "TimerManager.h"
#include "Components/EditableTextBox.h"

void ALobbyPC::BeginPlay()
{
	S2C_SetupWidget();
	SetInputMode(FInputModeGameAndUI());
}

void ALobbyPC::S2C_SetupWidget_Implementation()
{
	FStringClassReference LobbyWidgetRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/LobbyWidget.LobbyWidget_C'"));
	if (UClass* LobbyWidgetClass = LobbyWidgetRef.TryLoadClass<UUserWidget>())
	{
		LobbyWidget = CreateWidget<ULobbyWidgetBase>(this, LobbyWidgetClass);
		LobbyWidget->AddToViewport();
		if (!HasAuthority())
		{
			LobbyWidget->HideButton();
		}
		LobbyWidget->ChattingInput->SetUserFocus(this);
	}

	FStringClassReference LoadingWidgetRef(TEXT("WidgetBlueprint'/Game/Blueprints/Loading/LoadingWidget.LoadingWidget_C'"));
	if (UClass* LoadingWidgetClass = LoadingWidgetRef.TryLoadClass<UUserWidget>())
	{
		LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass);
		LoadingWidget->AddToViewport();
		LoadingWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ALobbyPC::StartGame()
{
	if (HasAuthority())
	{
		for (auto i = GetWorld()->GetControllerIterator(); i; ++i)
		{
			ALobbyPC* PC = Cast<ALobbyPC>(*i);
			if (PC)
			{
				PC->S2C_ViewLoading();
			}
		}
	}
	GetWorldTimerManager().SetTimer(StartGameTimer, this, &ALobbyPC::GameStartTimer, 1.0f);
}

void ALobbyPC::S2C_ViewLoading_Implementation()
{
	if (LoadingWidget)
	{
		LoadingWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ALobbyPC::GameStartTimer()
{
	GetWorld()->ServerTravel(TEXT("Battle"));
}

bool ALobbyPC::C2S_SendChatMessage_Validate(const FText& Message)
{
	return true;
}

void ALobbyPC::C2S_SendChatMessage_Implementation(const FText& Message)
{
	for (auto i = GetWorld()->GetControllerIterator(); i; ++i)
	{
		ALobbyPC* PC = Cast<ALobbyPC>(*i);
		if (PC)
		{
			PC->S2C_AddChatMessage(Message);
		}
	}
}

void ALobbyPC::S2C_AddChatMessage_Implementation(const FText & Message)
{
	//UE_LOG(LogClass, Warning, TEXT("%s"), *Message.ToString());
	if (LobbyWidget)
	{
		LobbyWidget->AddChatMessage(Message);
	}
}

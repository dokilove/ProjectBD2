// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyWidgetBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Lobby/LobbyPC.h"
#include "Kismet/GameplayStatics.h"
#include "BDGameInstance.h"

void ULobbyWidgetBase::NativeConstruct()
{
	PublicMessage = Cast<UTextBlock>(GetWidgetFromName("PublicMessage"));
	Alive = Cast<UTextBlock>(GetWidgetFromName("Alive"));
	ChattingBox = Cast<UScrollBox>(GetWidgetFromName("ChattingBox"));

	StartGameButton = Cast<UButton>(GetWidgetFromName("StartGameButton"));
	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::StartGame);
	}
	ChattingInput = Cast<UEditableTextBox>(GetWidgetFromName("ChattingInput"));
	if (ChattingInput)
	{
		ChattingInput->OnTextCommitted.AddDynamic(this,
			&ULobbyWidgetBase::OnTextCommitted);
	}
}

void ULobbyWidgetBase::StartGame()
{
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->StartGame();
	}
}

void ULobbyWidgetBase::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		if (CommitMethod == ETextCommit::OnEnter)
		{
			if (Text.IsEmpty())
			{

			}
			else
			{
				UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
				if (GI)
				{
					FString Message;
					Message =FString::Printf(TEXT("%s : %s"), *GI->UserID, *Text.ToString());
					//UE_LOG(LogClass, Warning, TEXT("%s"), *Message);
					PC->C2S_SendChatMessage(FText::FromString(*Message));
				}
			}
		}
	}
}

void ULobbyWidgetBase::HideButton()
{
	StartGameButton->SetVisibility(ESlateVisibility::Collapsed);
}

void ULobbyWidgetBase::AddChatMessage(const FText& Message)
{
	if (ChattingBox)
	{
	}
}


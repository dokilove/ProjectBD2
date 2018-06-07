// Fill out your copyright notice in the Description page of Project Settings.

#include "TitleWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

void UTitleWidgetBase::NativeConstruct()
{
	StartButton = Cast<UButton>(GetWidgetFromName("StartButton"));
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::StartServer);
	}
	LoginButton = Cast<UButton>(GetWidgetFromName("LoginButton"));
	if (LoginButton)
	{
		LoginButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::ConnectServer);
	}
	UserID = Cast<UEditableTextBox>(GetWidgetFromName("UserID"));
	ServerIP = Cast<UEditableTextBox>(GetWidgetFromName("ServerIP"));
}

void UTitleWidgetBase::StartServer()
{
	UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true, TEXT("listen"));
}

void UTitleWidgetBase::ConnectServer()
{
	if (ServerIP)
	{
		if (!ServerIP->GetText().IsEmpty())
		{
			UGameplayStatics::OpenLevel(this, *ServerIP->GetText().ToString());
		}
	}
}

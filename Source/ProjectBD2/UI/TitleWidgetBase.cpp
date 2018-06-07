// Fill out your copyright notice in the Description page of Project Settings.

#include "TitleWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "BDGameInstance.h"
#include "Title/TitlePC.h"

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
	if (UserID)
	{
		UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GI)
		{
			UserID->SetText(FText::FromString(*GI->UserID));
		}
	}
	ServerIP = Cast<UEditableTextBox>(GetWidgetFromName("ServerIP"));
}

void UTitleWidgetBase::StartServer()
{
	SetUserID();
	UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true, TEXT("listen"));
}

void UTitleWidgetBase::ConnectServer()
{
	if (ServerIP)
	{
		if (!ServerIP->GetText().IsEmpty())
		{
			SetUserID();
			UGameplayStatics::OpenLevel(this, *ServerIP->GetText().ToString());
		}
	}
}

void UTitleWidgetBase::SetUserID()
{
	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		if (UserID)
		{
			GI->UserID = UserID->GetText().ToString();
		}
	}
	ATitlePC* PC = Cast<ATitlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->ShowLoading();
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "TitlePC.h"
#include "UI/TitleWidgetBase.h"
#include "Http.h"
#include "Json.h"
#include "Misc/Base64.h"

void ATitlePC::BeginPlay()
{
	HttpConnection = &FHttpModule::Get();

	FStringClassReference TitleWidgetRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/TitleWidget.TitleWidget_C'"));

	if (UClass* TitleWidgetClass = TitleWidgetRef.TryLoadClass<UUserWidget>())
	{
		TitleWidget = CreateWidget<UTitleWidgetBase>(this, TitleWidgetClass);
		TitleWidget->AddToViewport();
	}

	FStringClassReference LoadingWidgetRef(TEXT("WidgetBlueprint'/Game/Blueprints/Loading/LoadingWidget.LoadingWidget_C'"));
	if (UClass* LoadingWidgetClass = LoadingWidgetRef.TryLoadClass<UUserWidget>())
	{
		LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass);
		LoadingWidget->AddToViewport();
		LoadingWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());


	LoginHTTPGet(TEXT("shim"), TEXT("123450"));
}

void ATitlePC::ShowLoading()
{
	LoadingWidget->SetVisibility(ESlateVisibility::Visible);
}

void ATitlePC::LoginHTTPGet(const FString & ID, const FString & Password)
{
	TSharedPtr<IHttpRequest> Request = HttpConnection->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ATitlePC::OnRequestComplete);

	// 한글이 안되어 한글 되게 바꿔주는 것
	// 나중에 한글 쓰는 부분 다 인코딩
	FString EncodeID = FGenericPlatformHttp::UrlEncode(ID);
	
	FString url = FString(TEXT("http://home.junios.net:8000/ue4/login.php"));
	url = url + FString::Printf(TEXT("?id=%s&password=%s"), *EncodeID, *Password);
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("text/plain"));
	Request->ProcessRequest();
}

void ATitlePC::LoginHTTPPost(const FString & ID, const FString & Password)
{

	// 이진 데이터를 텍스트로 바꾼다
	// FBase64::Encode(binary, Text);
}

void ATitlePC::OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (bSuccess)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString ID = JsonObject->GetStringField(TEXT("id"));
			int Password = (int)JsonObject->GetNumberField(TEXT("password"));

			GLog->Log(FString::Printf(TEXT("Parsing %s %d"), *ID, Password));
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Http.h"
#include "TitlePC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD2_API ATitlePC : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UTitleWidgetBase* TitleWidget;	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UUserWidget* LoadingWidget;

	void ShowLoading();

	class FHttpModule* HttpConnection;

	void LoginHTTPGet(const FString& ID, const FString& Password);

	void LoginHTTPPost(const FString& ID, const FString& Password);

	//UFUNCTION() -> X
	// UFUNCTION으로 하면 안됨
	void OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
	
};

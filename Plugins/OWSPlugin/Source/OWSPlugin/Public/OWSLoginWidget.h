// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "OWSLoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class OWSPLUGIN_API UOWSLoginWidget : public UUserWidget
{
	GENERATED_BODY()

	FHttpModule* Http;
	
public:	
	UPROPERTY(BlueprintReadWrite)
		FString RPGAPICustomerKey;

	UPROPERTY(BlueprintReadWrite, Category = "Config")
		FString RPGAPIPath = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Login")
		float LoginTimeout = 20.f;
	/*
	UFUNCTION(BlueprintCallable, Category = "Login")
	void Login(FString Email, FString Password);

	void OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Login")
		void ErrorLogin(const FString &ErrorMsg);
	*/

	UFUNCTION(BlueprintCallable, Category = "Login")
		void LoginAndCreateSession(FString Email, FString Password);

	void OnLoginAndCreateSessionResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "Login")
		void NotifyLoginAndCreateSession(const FString &UserSessionGUID);

	UFUNCTION(BlueprintImplementableEvent, Category = "Login")
		void ErrorLoginAndCreateSession(const FString &ErrorMsg);

	UFUNCTION(BlueprintCallable, Category = "Login")
		void OpenCreateAccountInBrowser();
	

};

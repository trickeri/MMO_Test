// Fill out your copyright notice in the Description page of Project Settings.

#include "OWSLoginWidget.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "OWSPlayerController.h"
#include "Runtime/Core/Public/Misc/ConfigCacheIni.h"


/*
UOWSLoginWidget::UOWSLoginWidget()
{
	LoginTimeout = 20.f;
}
*/
/*
void UOWSLoginWidget::Login(FString Email, FString Password)
{
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("RPGAPICustomerKey"),
		RPGAPICustomerKey,
		GGameIni
	);

	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("RPGAPIPath"),
		RPGAPIPath,
		GGameIni
	);

	Http = &FHttpModule::Get();
	Http->SetHttpTimeout(LoginTimeout); //Set timeout

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UOWSLoginWidget::OnLoginResponseReceived);
	//This is the url on which to process the request
	FString url = FString(TEXT("http://" + RPGAPIPath + "/Api/OWSCharacter?email=")) + Email + FString(TEXT("&password=")
		+ Password + FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey);
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UOWSLoginWidget::OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString ErrorMessage = JsonObject->GetStringField("ErrorMessage");

			if (!ErrorMessage.IsEmpty())
			{
				ErrorLogin(*ErrorMessage);
				return;
			}

			float PlayerLocationX = JsonObject->GetNumberField("LocationX");
			float PlayerLocationY = JsonObject->GetNumberField("LocationY");
			float PlayerLocationZ = JsonObject->GetNumberField("LocationZ");
			float PlayerRotationX = JsonObject->GetNumberField("RotationX");
			float PlayerRotationY = JsonObject->GetNumberField("RotationY");
			float PlayerRotationZ = JsonObject->GetNumberField("RotationZ");
			FString PlayerUserName = JsonObject->GetStringField("CharacterName");

			if (PlayerUserName.IsEmpty())
			{
				ErrorLogin(TEXT("User not found!"));
				return;
			}

			FString ServerIP = JsonObject->GetStringField("ServerIP");
			FString OnMapPort = JsonObject->GetStringField("OnMapPort");

			if (ServerIP.IsEmpty() || OnMapPort.IsEmpty())
			{
				ErrorLogin(TEXT("Error connecting to server!"));
				return;
			}

			//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, PlayerUserName);

			FString UE4url = ServerIP + FString(TEXT(":")) + OnMapPort
				+ FString(TEXT("?PLX=")) + FString::SanitizeFloat(PlayerLocationX)
				+ FString(TEXT("?PLY=")) + FString::SanitizeFloat(PlayerLocationY)
				+ FString(TEXT("?PLZ=")) + FString::SanitizeFloat(PlayerLocationZ)
				+ FString(TEXT("?PRX=")) + FString::SanitizeFloat(PlayerRotationX)
				+ FString(TEXT("?PRY=")) + FString::SanitizeFloat(PlayerRotationY)
				+ FString(TEXT("?PRZ=")) + FString::SanitizeFloat(PlayerRotationZ)
				+ FString(TEXT("?Player=")) + PlayerUserName;
			((AOWSPlayerController*)GetOwningPlayer())->TravelToMap(UE4url, false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnLoginResponseReceived Server returned no data!"));
			ErrorLogin(TEXT("Unknown login error!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnLoginResponseReceived Error accessing login server!"));
		ErrorLogin(TEXT("Unknown error connecting to server!"));
	}
}*/


void UOWSLoginWidget::LoginAndCreateSession(FString Email, FString Password)
{
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("RPGAPICustomerKey"),
		RPGAPICustomerKey,
		GGameIni
	);

	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("RPGAPIPath"),
		RPGAPIPath,
		GGameIni
	);

	Http = &FHttpModule::Get();
	Http->SetHttpTimeout(LoginTimeout); //Set timeout

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UOWSLoginWidget::OnLoginAndCreateSessionResponseReceived);
	//This is the url on which to process the request
	FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGUser/LoginAndCreateSession/"));

	FString PostParameters = FString(TEXT("id=")) + Email
		+ FString(TEXT("&Password=")) + Password
		+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void UOWSLoginWidget::OnLoginAndCreateSessionResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString ErrorMessage = JsonObject->GetStringField("ErrorMessage");

			if (!ErrorMessage.IsEmpty())
			{
				ErrorLoginAndCreateSession(*ErrorMessage);
				return;
			}

			FString UserSessionGUID = JsonObject->GetStringField("UserSessionGUID");

			NotifyLoginAndCreateSession(UserSessionGUID);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnUserLoginResponseReceived Server returned no data!"));
			ErrorLoginAndCreateSession(TEXT("Unknown login error!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnUserLoginResponseReceived Error accessing login server!"));
		ErrorLoginAndCreateSession(TEXT("Unknown error connecting to server!"));
	}
}




void UOWSLoginWidget::OpenCreateAccountInBrowser()
{
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("RPGAPICustomerKey"),
		RPGAPICustomerKey,
		GGameIni
	);

	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("RPGAPIPath"),
		RPGAPIPath,
		GGameIni
	);

	FString TheURL = "http://" + RPGAPIPath + "/Login/CreateAccount/" + RPGAPICustomerKey;
	FPlatformProcess::LaunchURL(*TheURL, nullptr, nullptr);
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "OWSGameMode.h"
#include "OWSGameInstance.h"
#include "OWSPlayerState.h"
#include "OWSPlayerController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Core/Public/Misc/ConfigCacheIni.h"

AOWSGameMode::AOWSGameMode()
{
	InactivePlayerStateLifeSpan = 1;

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
}

void AOWSGameMode::StartPlay()
{
	Super::StartPlay();

	GetAllInventoryItems();

	if (Role == ROLE_Authority)
	{
		//Lookup which Zone this server is running for and get the ZoneName into IAmZoneName var
		GetServerInstanceFromPort();

		if (GetCharactersOnlineIntervalInSeconds > 0.f)
		{
			GetWorld()->GetTimerManager().SetTimer(OnGetAllCharactersOnlineTimerHandle, this, &AOWSGameMode::GetAllCharactersOnline, GetCharactersOnlineIntervalInSeconds, true);
		}

		if (UpdateServerStatusEveryXSeconds > 0.f)
		{
			GetWorld()->GetTimerManager().SetTimer(UpdateServerStatusEveryXSecondsTimerHandle, this, &AOWSGameMode::UpdateNumberOfPlayers, UpdateServerStatusEveryXSeconds, true);
		}
	}
}

FString AOWSGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString retString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	//FString UserSessionGUID = UGameplayStatics::ParseOption(Options, TEXT("UserSessionGUID"));

	/*NewPlayerController->PlayerState->PlayerName = UserSessionGUID;

	if (UserSessionGUID == "")
	{
		NewPlayerController->PlayerState->PlayerName = DebugCharacterName;
	}*/

	UE_LOG(LogTemp, Warning, TEXT("InitNewPlayer Started"));

	//AOWSPlayerController* OWSPlayerController = CastChecked<AOWSPlayerController>(NewPlayerController);
	/*
	FString PlayerName1 = "";
	float fPLX = 0.f;
	float fPLY = 0.f;
	float fPLZ = 0.f;
	float fPRX = 0.f;
	float fPRY = 0.f;
	float fPRZ = 0.f;
	FString ErrorMessage = "";

	OWSPlayerController->GetUserSession(UserSessionGUID, PlayerName1, fPLX, fPLY, fPLZ, fPRX, fPRY, fPRZ, ErrorMessage);*/

	UE_LOG(LogTemp, Warning, TEXT("Raw options: %s"), *Options);

	FString DecodedOptions = FGenericPlatformHttp::UrlDecode(Options);

	UE_LOG(LogTemp, Warning, TEXT("Decoded options: %s"), *DecodedOptions);
	
	FString PLX = UGameplayStatics::ParseOption(DecodedOptions, TEXT("PLX"));
	FString PLY = UGameplayStatics::ParseOption(DecodedOptions, TEXT("PLY"));
	FString PLZ = UGameplayStatics::ParseOption(DecodedOptions, TEXT("PLZ"));
	FString PRX = UGameplayStatics::ParseOption(DecodedOptions, TEXT("PRX"));
	FString PRY = UGameplayStatics::ParseOption(DecodedOptions, TEXT("PRY"));
	FString PRZ = UGameplayStatics::ParseOption(DecodedOptions, TEXT("PRZ"));
	FString PlayerName1 = UGameplayStatics::ParseOption(DecodedOptions, TEXT("Player"));

	UE_LOG(LogTemp, Warning, TEXT("PlayerName1: %s"), *PlayerName1);

	FString OWSDefaultPawnClass = UGameplayStatics::ParseOption(DecodedOptions, TEXT("DPC"));

	if (PlayerName1 == "")
	{
		PlayerName1 = DebugCharacterName;
	}

	float fPLX = FCString::Atof(*PLX);
	float fPLY = FCString::Atof(*PLY);
	float fPLZ = FCString::Atof(*PLZ);
	float fPRX = FCString::Atof(*PRX);
	float fPRY = FCString::Atof(*PRY);
	float fPRZ = FCString::Atof(*PRZ);

	UE_LOG(LogTemp, Warning, TEXT("Incoming start location is %f, %f, %f"), fPLX, fPLY, fPLZ);

	AOWSPlayerState* NewPlayerState = CastChecked<AOWSPlayerState>(NewPlayerController->PlayerState);	

	if (fPLX != 0 || fPLY != 0 || fPLZ != 0)
	{
		NewPlayerState->PlayerStartLocation.X = fPLX;
		NewPlayerState->PlayerStartLocation.Y = fPLY;
		NewPlayerState->PlayerStartLocation.Z = fPLZ;
		NewPlayerState->PlayerStartRotation.Roll = fPRX;
		NewPlayerState->PlayerStartRotation.Pitch = fPRY;
		NewPlayerState->PlayerStartRotation.Yaw = fPRZ;
	}
	else
	{
		NewPlayerState->PlayerStartLocation.X = DebugStartLocation.X;
		NewPlayerState->PlayerStartLocation.Y = DebugStartLocation.Y;
		NewPlayerState->PlayerStartLocation.Z = DebugStartLocation.Z;
		NewPlayerState->PlayerStartRotation.Roll = 0;
		NewPlayerState->PlayerStartRotation.Pitch = 0;
		NewPlayerState->PlayerStartRotation.Yaw = 0;
	}

	NewPlayerState->SetPlayerName(PlayerName1);
	NewPlayerState->DefaultPawnClass = OWSDefaultPawnClass;

	return retString;
}

APawn * AOWSGameMode::SpawnDefaultPawnFor_Implementation(AController * NewPlayer, class AActor * StartSpot)
{
	//UE_LOG(LogTemp, Warning, TEXT("Start SpawnDefaultPawnFor"));

	AOWSPlayerState* NewPlayerState = CastChecked<AOWSPlayerState>(NewPlayer->PlayerState);
	UE_LOG(LogTemp, Warning, TEXT("Spawn Location is %f, %f, %f"), NewPlayerState->PlayerStartLocation.X, NewPlayerState->PlayerStartLocation.Y, NewPlayerState->PlayerStartLocation.Z);
	UE_LOG(LogTemp, Warning, TEXT("Spawn Rotation is %f, %f, %f"), NewPlayerState->PlayerStartRotation.Roll, NewPlayerState->PlayerStartRotation.Pitch, NewPlayerState->PlayerStartRotation.Yaw);

	//StartSpot->SetActorLocation(NewPlayerState->PlayerStartLocation);
	//StartSpot->SetActorRotation(NewPlayerState->PlayerStartRotation);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = Instigator;
	SpawnInfo.ObjectFlags |= RF_Transient;

	SpawnInfo.bDeferConstruction = false;
	APawn *  retPawn;

	/*if (!NewPlayerState->DefaultPawnClass.IsEmpty())
	{
		//Example: /Game/ThirdPersonBP/Blueprints/BlueprintName.BlueprintName
		UE_LOG(LogTemp, Warning, TEXT("Attempting to spawn custom pawn in SpawnDefaultPawnFor_Implementation: %s"), *NewPlayerState->DefaultPawnClass);
		retPawn = GetWorld()->SpawnActor<APawn>(LoadClass<APawn>(NULL, *NewPlayerState->DefaultPawnClass, NULL, LOAD_None, NULL), NewPlayerState->PlayerStartLocation, FRotator::ZeroRotator, SpawnInfo);
	}
	else
	{*/
		retPawn = GetWorld()->SpawnActor<APawn>(GetDefaultPawnClassForController(NewPlayer), NewPlayerState->PlayerStartLocation, NewPlayerState->PlayerStartRotation, SpawnInfo);
	//}

	if (retPawn == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't spawn Pawn in SpawnDefaultPawnFor_Implementation"));
	}

	return retPawn;
}



void AOWSGameMode::GetAllInventoryItems()
{
	Http = &FHttpModule::Get();

	/*if (GEngine->GetWorld() != nullptr && GEngine->GetWorld()->GetGameInstance() != nullptr)
	{
		UOWSGameInstance* gameInstance = Cast<UOWSGameInstance>(GEngine->GetWorld()->GetGameInstance());
		Http = gameInstance->Http;
	}*/

	if (!RPGAPICustomerKey.IsEmpty())
	{
		TSharedRef<IHttpRequest> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSGameMode::OnGetAllInventoryItemsResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGInventory/GetAllInventoryItems/"));

		FString PostParameters = FString(TEXT("CustomerGUID=")) + RPGAPICustomerKey;

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSGameMode::OnGetAllInventoryItemsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			if (JsonObject->HasField("rows"))
			{
				TArray<TSharedPtr<FJsonValue>> Rows = JsonObject->GetArrayField("rows");

				for (int RowNum = 0; RowNum != Rows.Num(); RowNum++) {
					FInventoryItemStruct tempInventoryItem;
					TSharedPtr<FJsonObject> tempRow = Rows[RowNum]->AsObject();
					tempInventoryItem.ItemName = tempRow->GetStringField("ItemName");
					tempInventoryItem.ItemDescription = tempRow->GetStringField("ItemDescription");
					tempInventoryItem.ItemValue = tempRow->GetIntegerField("ItemValue");
					tempInventoryItem.ItemCanStack = tempRow->GetBoolField("ItemCanStack");
					tempInventoryItem.ItemStackSize = tempRow->GetIntegerField("ItemStackSize");
					tempInventoryItem.IsUsable = tempRow->GetBoolField("ItemIsUsable");
					tempInventoryItem.IsConsumedOnUse = tempRow->GetBoolField("ItemIsConsumedOnUse");
					//tempInventoryItem.DefaultNumberOfUses = tempRow->GetIntegerField("DefaultNumberOfUses");
					tempInventoryItem.ItemWeight = (float)tempRow->GetNumberField("ItemWeight");

					tempInventoryItem.ItemTypeID = tempRow->GetIntegerField("ItemTypeID");
					tempInventoryItem.ItemTypeDescription = tempRow->GetStringField("ItemTypeDesc");
					tempInventoryItem.ItemTypeQuality = tempRow->GetIntegerField("ItemTypeQuality");

					tempInventoryItem.UserItemType = tempRow->GetIntegerField("UserItemType");
					tempInventoryItem.EquipmentType = tempRow->GetIntegerField("EquipmentType");
					tempInventoryItem.EquipmentSlotType = tempRow->GetIntegerField("EquipmentSlotType");
					tempInventoryItem.ItemTier = tempRow->GetIntegerField("ItemTier");
					tempInventoryItem.ItemQuality = tempRow->GetIntegerField("ItemQuality");
					tempInventoryItem.ItemDuration = tempRow->GetIntegerField("ItemDuration");
					tempInventoryItem.CanBeDropped = tempRow->GetBoolField("CanBeDropped");
					tempInventoryItem.CanBeDestroyed = tempRow->GetBoolField("CanBeDestroyed");

					tempInventoryItem.CustomData = tempRow->GetStringField("CustomData");
					//tempInventoryItem.ItemMesh = tempRow->GetStringField("ItemMesh");
					//tempInventoryItem.MeshToUseForPickup = tempRow->GetStringField("MeshToUseForPickup");					
					tempInventoryItem.PremiumCurrencyPrice = tempRow->GetIntegerField("PremiumCurrencyPrice");
					tempInventoryItem.FreeCurrencyPrice = tempRow->GetIntegerField("FreeCurrencyPrice");

					tempInventoryItem.WeaponActorClassPath = tempRow->GetStringField("WeaponActorClass");
					tempInventoryItem.StaticMeshPath = tempRow->GetStringField("StaticMesh");
					tempInventoryItem.SkeletalMeshPath = tempRow->GetStringField("SkeletalMesh");
					tempInventoryItem.TextureToUseForIcon = tempRow->GetStringField("TextureToUseForIcon");

					AllInventoryItems.Add(tempInventoryItem);
					NotifyGetAllInventoryItems();
					BroadcastItemLibraryLoaded();
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnGetAllInventoryItemsResponseReceived Server returned no data!"));
			ErrorGetAllInventoryItems(TEXT("OnGetAllInventoryItemsResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnGetAllInventoryItemsResponseReceived Error accessing server!"));
		ErrorGetAllInventoryItems(TEXT("OnGetAllInventoryItemsResponseReceived Error accessing server!"));
	}
}


void AOWSGameMode::GetGlobalDataItem(FString GlobalDataKey)
{
	Http = &FHttpModule::Get();

	if (!RPGAPICustomerKey.IsEmpty())
	{
		TSharedRef<IHttpRequest> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSGameMode::OnGetGlobalDataItemResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/GetGlobalDataItem/")) + GlobalDataKey;

		FString PostParameters = FString(TEXT("CustomerGUID=")) + RPGAPICustomerKey;

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSGameMode::OnGetGlobalDataItemResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString GlobalDataValue = JsonObject->GetStringField("GlobalDataValue");;

			NotifyGetGlobalDataItem(GlobalDataValue);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnGetGlobalDataItemResponseReceived Server returned no data!"));
			ErrorGetGlobalDataItem(TEXT("OnGetGlobalDataItemResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnGetGlobalDataItemResponseReceived Error accessing server!"));
		ErrorGetGlobalDataItem(TEXT("OnGetGlobalDataItemResponseReceived Error accessing server!"));
	}
}


void AOWSGameMode::AddOrUpdateGlobalDataItem(FString GlobalDataKey, FString GlobalDataValue)
{
	Http = &FHttpModule::Get();

	if (!RPGAPICustomerKey.IsEmpty())
	{
		TSharedRef<IHttpRequest> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSGameMode::OnAddOrUpdateGlobalDataItemResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/AddOrUpdateGlobalDataItem/")) + GlobalDataKey;

		FString PostParameters = FString(TEXT("GlobalDataValue=")) + GlobalDataValue
			+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSGameMode::OnAddOrUpdateGlobalDataItemResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		NotifyAddOrUpdateGlobalDataItem();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnAddOrUpdateGlobalDataItemResponseReceived Error accessing server!"));
		ErrorAddOrUpdateGlobalDataItem(TEXT("OnAddOrUpdateGlobalDataItemResponseReceived Error accessing server!"));
	}
}



void AOWSGameMode::SaveAllPlayerLocations()
{
	Http = &FHttpModule::Get();

	FString DataToSave;

	int PlayerIndex = 0;

	if (NextSaveGroupIndex < SplitSaveIntoHowManyGroups)
	{
		NextSaveGroupIndex++;
	}
	else
	{
		NextSaveGroupIndex = 0;
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (NextSaveGroupIndex == PlayerIndex % SplitSaveIntoHowManyGroups)
		{
			APawn* MyPawn = Iterator->Get()->GetPawn();

			if (MyPawn)
			{
				AOWSCharacterBase* MyCharacter = Cast<AOWSCharacterBase>(MyPawn);

				if (MyCharacter)
				{
					FVector PawnLocation = MyPawn->GetActorLocation();
					FRotator PawnRotation = MyPawn->GetActorRotation();

					DataToSave.Append(*Iterator->Get()->PlayerState->GetPlayerName());
					DataToSave.Append(":");
					DataToSave.Append(FString::SanitizeFloat(PawnLocation.X));
					DataToSave.Append(":");
					DataToSave.Append(FString::SanitizeFloat(PawnLocation.Y));
					DataToSave.Append(":");
					DataToSave.Append(FString::SanitizeFloat(PawnLocation.Z));
					DataToSave.Append(":");
					DataToSave.Append(FString::SanitizeFloat(PawnRotation.Roll));
					DataToSave.Append(":");
					DataToSave.Append(FString::SanitizeFloat(PawnRotation.Pitch));
					DataToSave.Append(":");
					DataToSave.Append(FString::SanitizeFloat(PawnRotation.Yaw));
					DataToSave.Append("|");
				}
			}
		}

		//UE_LOG(LogTemp, Error, TEXT("PlayerController: %s - %f, %f, %f"), *Iterator->Get()->PlayerState->PlayerName, PawnLocation.X, PawnLocation.Y, PawnLocation.Z);
		PlayerIndex++;
	}

	if (DataToSave.Len() < 1)
	{
		return;
	}

	DataToSave = DataToSave.Left(DataToSave.Len() - 1);

	//UE_LOG(LogTemp, Log, TEXT("DataToSave: %s"), *DataToSave);

	/*if (GEngine->GetWorld() != nullptr && GEngine->GetWorld()->GetGameInstance() != nullptr)
	{
		UOWSGameInstance* gameInstance = Cast<UOWSGameInstance>(GEngine->GetWorld()->GetGameInstance());
		Http = gameInstance->Http;
	}*/

	//UE_LOG(LogTemp, Error, TEXT("%s"), *PlayerState->PlayerName);
	//FString PlayerName = "Test";
	/*FString PlayerName = PlayerState->PlayerName;
	PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

	AOWSCharacter* MyRPGCharacter = Cast<AOWSCharacter>(GetPawn());*/

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AOWSGameMode::OnSaveAllPlayerLocationsResponseReceived);
	//This is the url on which to process the request
	FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/UpdateAllPlayerPositions/"));


	FString PostParameters = FString(TEXT("Data=")) + DataToSave
		+ FString(TEXT("&MapName=")) 
		+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void AOWSGameMode::OnSaveAllPlayerLocationsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		//UE_LOG(LogTemp, Log, TEXT("OnSaveAllPlayerLocationsResponseReceived Success!"));

		//UE_LOG(LogTemp, Error, TEXT("OnSavePlayerLocationResponseReceived Success: %s"), *Response->GetContentAsString());
		/*TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{

		}
		else
		{
		UE_LOG(LogTemp, Error, TEXT("OnSaveAllPlayerLocationsResponseReceived Server returned no data!"));
		}*/
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnSaveAllPlayerLocationsResponseReceived Error accessing server!"));
	}
}


void AOWSGameMode::GetAllCharactersOnline()
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AOWSGameMode::OnGetAllCharactersOnlineResponseReceived);
	//This is the url on which to process the request
	FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/GetAllCharactersOnline/"));

	FString PostParameters = FString(TEXT("CustomerGUID=")) + RPGAPICustomerKey;

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void AOWSGameMode::OnGetAllCharactersOnlineResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			//TArray<FCharactersOnlineStruct> tempCharactersOnline;

			CharactersOnline.Empty(CharactersOnline.Num());

			if (JsonObject->HasField("rows"))
			{
				TArray<TSharedPtr<FJsonValue>> Rows = JsonObject->GetArrayField("rows");

				for (int RowNum = 0; RowNum != Rows.Num(); RowNum++) {
					FCharactersOnlineStruct tempCharacterOnline;
					TSharedPtr<FJsonObject> tempRow = Rows[RowNum]->AsObject();

					tempCharacterOnline.CharacterID = tempRow->GetIntegerField("CharacterID");
					tempCharacterOnline.CharName = tempRow->GetStringField("CharName");
					tempCharacterOnline.CharacterLevel = tempRow->GetIntegerField("CharacterLevel");
					tempCharacterOnline.Gender = tempRow->GetIntegerField("Gender");
					tempCharacterOnline.Alignment = tempRow->GetIntegerField("Alignment");
					tempCharacterOnline.Fame = tempRow->GetIntegerField("Fame");
					tempCharacterOnline.TeamNumber = tempRow->GetIntegerField("TeamNumber");
				
					CharactersOnline.Add(tempCharacterOnline);
				}
			}
			//NotifyGetAllCharactersOnline(CharactersOnline);

			//UE_LOG(LogTemp, Error, TEXT("Total number of players online: %d"), CharactersOnline.Num());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnGetAllCharactersOnlineResponseReceived Server returned no data!"));
			ErrorGetAllCharactersOnline(TEXT("OnGetAllCharactersOnlineResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnGetAllCharactersOnlineResponseReceived Error accessing server!"));
		ErrorGetAllCharactersOnline(TEXT("OnGetAllCharactersOnlineResponseReceived Error accessing server!"));
	}
}

bool AOWSGameMode::IsPlayerOnline(FString CharacterName)
{
	return CharactersOnline.ContainsByPredicate([&](FCharactersOnlineStruct Result) {return CharacterName == Result.CharName; });
}


void AOWSGameMode::GetZoneInstancesForZone(FString ZoneName)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AOWSGameMode::OnGetZoneInstancesForZoneResponseReceived);
	//This is the url on which to process the request
	FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/GetZoneInstancesForZone/"));

	FString PostParameters = FString(TEXT("ZoneName=")) + ZoneName
		+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void AOWSGameMode::OnGetZoneInstancesForZoneResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			TArray<FZoneInstanceStruct> ZoneInstances;

			if (JsonObject->HasField("rows"))
			{
				TArray<TSharedPtr<FJsonValue>> Rows = JsonObject->GetArrayField("rows");

				for (int RowNum = 0; RowNum != Rows.Num(); RowNum++) {
					FZoneInstanceStruct tempZoneInstance;
					TSharedPtr<FJsonObject> tempRow = Rows[RowNum]->AsObject();

					tempZoneInstance.MapInstanceID = tempRow->GetIntegerField("MapInstanceID");
					tempZoneInstance.Port = tempRow->GetIntegerField("Port");
					tempZoneInstance.Status = tempRow->GetIntegerField("Status");
					tempZoneInstance.ZoneName = tempRow->GetStringField("ZoneName");
					tempZoneInstance.MapName = tempRow->GetStringField("MapName");

					ZoneInstances.Add(tempZoneInstance);
				}
			}
			NotifyGetZoneInstancesForZone(ZoneInstances);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnGetZoneInstancesForZoneResponseReceived Server returned no data!"));
			ErrorGetZoneInstancesForZone(TEXT("OnGetZoneInstancesForZoneResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnGetZoneInstancesForZoneResponseReceived Error accessing server!"));
		ErrorGetZoneInstancesForZone(TEXT("OnGetZoneInstancesForZoneResponseReceived Error accessing server!"));
	}
}


void AOWSGameMode::GetServerInstanceFromPort()
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AOWSGameMode::OnGetServerInstanceFromPortResponseReceived);
	//This is the url on which to process the request
	FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/GetServerInstanceFromPort/"));

	FString Port = GetWorld()->GetAddressURL();
	Port = Port.Replace(TEXT(":"), TEXT(""));

	FString PostParameters = FString(TEXT("Port=")) + Port
		+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void AOWSGameMode::OnGetServerInstanceFromPortResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			TArray<FZoneInstanceStruct> ZoneInstances;

			if (JsonObject->HasField("rows"))
			{
				TArray<TSharedPtr<FJsonValue>> Rows = JsonObject->GetArrayField("rows");

				TSharedPtr<FJsonObject> tempRow = Rows[0]->AsObject();

				FString ZoneName = tempRow->GetStringField("ZoneName");

				IAmZoneName = ZoneName;
				UE_LOG(LogTemp, Warning, TEXT("I am ZoneName: %s"), *IAmZoneName);

				NotifyGetServerInstanceFromPort(ZoneName);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("OnGetServerInstanceFromPortResponseReceived No Rows!  Ignore this error if you are running from the editor!"));
				ErrorGetServerInstanceFromPort(TEXT("OnGetServerInstanceFromPortResponseReceived No Rows!  Ignore this error if you are running from the editor!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnGetServerInstanceFromPortResponseReceived Server returned no data!"));
			ErrorGetServerInstanceFromPort(TEXT("OnGetServerInstanceFromPortResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnGetServerInstanceFromPortResponseReceived Error accessing server!"));
		ErrorGetServerInstanceFromPort(TEXT("OnGetServerInstanceFromPortResponseReceived Error accessing server!"));
	}
}




void AOWSGameMode::UpdateNumberOfPlayers()
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AOWSGameMode::OnUpdateNumberOfPlayersResponseReceived);
	//This is the url on which to process the request
	FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/UpdateNumberOfPlayers/"));

	FString NumberOfConnectedPlayers = FString::FromInt(NumPlayers);

	FString Port = GetWorld()->GetAddressURL();
	Port = Port.Replace(TEXT(":"), TEXT(""));

	FString PostParameters = FString(TEXT("Port=")) + Port
		+ FString(TEXT("&NumberOfConnectedPlayers=")) + NumberOfConnectedPlayers
		+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}


void AOWSGameMode::OnUpdateNumberOfPlayersResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnUpdateNumberOfPlayersResponseReceived Error accessing server!"));
	}
}


FString AOWSGameMode::GetAddressURLAndPort()
{
	return GetWorld()->GetAddressURL();
}
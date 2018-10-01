// Fill out your copyright notice in the Description page of Project Settings.

#include "OWSCharacterWithAbilities.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Runtime/Core/Public/Misc/Guid.h"
#include "OWSPlayerController.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsModule.h"
#include "OWSAttributeSet.h"
//#include "OWSGameplayAbility.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"

#define CONSTRUCT_CLASS(Class, Name) Class* Name = NewObject<Class>(GetTransientPackage(), FName(TEXT(#Name)))

AOWSCharacterWithAbilities::AOWSCharacterWithAbilities()
{
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);

	SpellAbilityHandles.SetNum(10, false);
}


void AOWSCharacterWithAbilities::BeginPlay()
{
	Super::BeginPlay();

	OWSAttributes = NewObject<UOWSAttributeSet>();
	
	if (Role == ROLE_Authority)
	{
		GetWorld()->GetTimerManager().SetTimer(OnGetCharacterStatsTimerHandle, this, &AOWSCharacterWithAbilities::GetCharacterStats, 1.0f, true);
	}
}

void AOWSCharacterWithAbilities::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AbilitySystem->InitStats(
		UOWSAttributeSet::StaticClass(), NULL);
}

void AOWSCharacterWithAbilities::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	AbilitySystem->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds("ConfirmInput", "CancelInput", "AbilityInput"));
}

void AOWSCharacterWithAbilities::ChangeSpell(int SpellSlot, TSubclassOf<class UGameplayAbility> NewSpell)
{
	if (AbilitySystem)
	{
		if (HasAuthority())
		{
			if (NewSpell && SpellSlot > 0 && SpellSlot < SpellAbilityHandles.Num())
			{
				AbilitySystem->ClearAbility(SpellAbilityHandles[SpellSlot - 1]);

				SpellAbilityHandles[SpellSlot - 1] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(NewSpell.GetDefaultObject(), 1, SpellSlot - 1));
			}
		}
	}
}

void AOWSCharacterWithAbilities::GrantAbility(TSubclassOf<class UGameplayAbility> NewAbility)
{
	if (AbilitySystem)
	{
		if (HasAuthority())
		{
			if (NewAbility)
			{
				AbilitySystem->GiveAbility(FGameplayAbilitySpec(NewAbility.GetDefaultObject(), 1));
			}
		}
	}
}

void AOWSCharacterWithAbilities::GrantAbilityKeyBind(TSubclassOf<class UGameplayAbility> NewAbility, int AbilityLevel, int InputID)
{
	if (AbilitySystem)
	{
		if (HasAuthority())
		{
			if (NewAbility)
			{
				AbilitySystem->GiveAbility(FGameplayAbilitySpec(NewAbility.GetDefaultObject(), AbilityLevel, InputID));
			}
		}
	}
}

void AOWSCharacterWithAbilities::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Initialize our abilities
	if (AbilitySystem)
	{
		AbilitySystem->InitAbilityActorInfo(this, this);
		AddDefaultGameplayAbilities();
	}
}

void AOWSCharacterWithAbilities::OnRep_Controller()
{
	Super::OnRep_Controller();

	// Our controller changed, must update ActorInfo on AbilitySystemComponent
	if (AbilitySystem)
	{
		AbilitySystem->RefreshAbilityActorInfo();
	}
}

void AOWSCharacterWithAbilities::AddDefaultGameplayAbilities()
{
	if (AbilitySystem)
	{
		if (HasAuthority())
		{
			if (Weapon1)
			{
				AbilitySystem->GiveAbility(FGameplayAbilitySpec(Weapon1.GetDefaultObject(), 1, 10));
			}

			if (Weapon2)
			{
				AbilitySystem->GiveAbility(FGameplayAbilitySpec(Weapon2.GetDefaultObject(), 1, 11));
			}

			if (Ability1)
			{
				SpellAbilityHandles[0] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability1.GetDefaultObject(), 1, 0));
			}

			if (Ability2)
			{
				SpellAbilityHandles[1] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability2.GetDefaultObject(), 1, 1));
			}

			if (Ability3)
			{
				SpellAbilityHandles[2] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability3.GetDefaultObject(), 1, 2));
			}

			if (Ability4)
			{
				SpellAbilityHandles[3] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability4.GetDefaultObject(), 1, 3));
			}

			if (Ability5)
			{
				SpellAbilityHandles[4] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability5.GetDefaultObject(), 1, 4));
			}

			if (Ability6)
			{
				SpellAbilityHandles[5] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability6.GetDefaultObject(), 1, 5));
			}

			if (Ability7)
			{
				SpellAbilityHandles[6] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability7.GetDefaultObject(), 1, 6));
			}

			if (Ability8)
			{
				SpellAbilityHandles[7] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability8.GetDefaultObject(), 1, 7));
			}

			if (Ability9)
			{
				SpellAbilityHandles[8] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability9.GetDefaultObject(), 1, 8));
			}

			if (Ability0)
			{
				SpellAbilityHandles[9] = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability0.GetDefaultObject(), 1, 9));
			}
		}
	}
}

void AOWSCharacterWithAbilities::GetCharacterStats()
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		GetWorld()->GetTimerManager().ClearTimer(OnGetCharacterStatsTimerHandle);
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacterWithAbilities::OnGetCharacterStatsResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/api/RPGCharacter/GetByCharName"));

		FString PostParameters = TEXT("{ \"id\": \"") + PlayerName
			+ FString(TEXT("\", \"CustomerGUID\": \"")) + RPGAPICustomerKey + TEXT("\"");

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/json"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacterWithAbilities::OnGetCharacterStatsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			/*
			CharacterName = JsonObject->GetStringField("CharacterName");
			CharacterLevel = JsonObject->GetNumberField("Level");
			Gender = JsonObject->GetNumberField("Gender");
			XP = JsonObject->GetNumberField("XP");
			HitDice = JsonObject->GetNumberField("HitDie");
			MaxHP = JsonObject->GetNumberField("MaxHealth");
			Wounds = JsonObject->GetNumberField("Wounds");
			*/

			static UProperty* ThirstProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Thirst));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ThirstProperty), JsonObject->GetNumberField("Thirst"));
			static UProperty* HungerProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Hunger));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(HungerProperty), JsonObject->GetNumberField("Hunger"));

			static UProperty* MaxHealthProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, MaxHealth));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MaxHealthProperty), JsonObject->GetNumberField("MaxHealth"));
			static UProperty* HealthProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Health));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(HealthProperty), JsonObject->GetNumberField("Health"));
			static UProperty* HealthRegenRateProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, HealthRegenRate));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(HealthRegenRateProperty), JsonObject->GetNumberField("HealthRegenRate"));

			static UProperty* MaxManaProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, MaxMana));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MaxManaProperty), JsonObject->GetNumberField("MaxMana"));
			static UProperty* ManaProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Mana));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ManaProperty), JsonObject->GetNumberField("Mana"));
			static UProperty* ManaRegenRateProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, ManaRegenRate));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ManaRegenRateProperty), JsonObject->GetNumberField("ManaRegenRate"));

			static UProperty* MaxEnergyProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, MaxEnergy));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MaxEnergyProperty), JsonObject->GetNumberField("MaxEnergy"));
			static UProperty* EnergyProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Energy));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(EnergyProperty), JsonObject->GetNumberField("Energy"));
			static UProperty* EnergyRegenRateProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, EnergyRegenRate));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(EnergyRegenRateProperty), JsonObject->GetNumberField("EnergyRegenRate"));

			static UProperty* MaxFatigueProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, MaxFatigue));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MaxFatigueProperty), JsonObject->GetNumberField("MaxFatigue"));
			static UProperty* FatigueProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Fatigue));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(FatigueProperty), JsonObject->GetNumberField("Fatigue"));
			static UProperty* FatigueRegenRateProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, FatigueRegenRate));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(FatigueRegenRateProperty), JsonObject->GetNumberField("FatigueRegenRate"));

			static UProperty* MaxStaminaProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, MaxStamina));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MaxStaminaProperty), JsonObject->GetNumberField("MaxStamina"));
			static UProperty* StaminaProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Stamina));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(StaminaProperty), JsonObject->GetNumberField("Stamina"));
			static UProperty* StaminaRegenRateProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, StaminaRegenRate));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(StaminaRegenRateProperty), JsonObject->GetNumberField("StaminaRegenRate"));
			
			static UProperty* MaxEnduranceProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, MaxEndurance));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MaxEnduranceProperty), JsonObject->GetNumberField("MaxEndurance"));
			static UProperty* EnduranceProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Endurance));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(EnduranceProperty), JsonObject->GetNumberField("Endurance"));
			static UProperty* EnduranceRegenRateProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, EnduranceRegenRate));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(EnduranceRegenRateProperty), JsonObject->GetNumberField("EnduranceRegenRate"));

			static UProperty* StrengthProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Strength));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(StrengthProperty), JsonObject->GetNumberField("Strength"));
			static UProperty* DexterityProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Dexterity));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(DexterityProperty), JsonObject->GetNumberField("Dexterity"));
			static UProperty* ConstitutionProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Constitution));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ConstitutionProperty), JsonObject->GetNumberField("Constitution"));
			static UProperty* IntellectProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Intellect));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(IntellectProperty), JsonObject->GetNumberField("Intellect"));
			static UProperty* WisdomProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Wisdom));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(WisdomProperty), JsonObject->GetNumberField("Wisdom"));
			static UProperty* CharismaProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Charisma));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(CharismaProperty), JsonObject->GetNumberField("Charisma"));
			static UProperty* SpiritProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Spirit));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(SpiritProperty), JsonObject->GetNumberField("Spirit"));
			static UProperty* MagicProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Magic));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MagicProperty), JsonObject->GetNumberField("Magic"));
			static UProperty* FortitudeProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Fortitude));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(FortitudeProperty), JsonObject->GetNumberField("Fortitude"));
			static UProperty* ReflexProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Reflex));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ReflexProperty), JsonObject->GetNumberField("Reflex"));
			static UProperty* WillpowerProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Willpower));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(WillpowerProperty), JsonObject->GetNumberField("Willpower"));

			static UProperty* BaseAttackProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, BaseAttack));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(BaseAttackProperty), JsonObject->GetNumberField("BaseAttack"));
			static UProperty* BaseAttackBonusProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, BaseAttackBonus));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(BaseAttackBonusProperty), JsonObject->GetNumberField("BaseAttackBonus"));
			static UProperty* AttackPowerProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, AttackPower));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(AttackPowerProperty), JsonObject->GetNumberField("AttackPower"));
			static UProperty* AttackSpeedProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, AttackSpeed));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(AttackSpeedProperty), JsonObject->GetNumberField("AttackSpeed"));
			static UProperty* CritChanceProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, CritChance));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(CritChanceProperty), JsonObject->GetNumberField("CritChance"));
			static UProperty* CritMultiplierProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, CritMultiplier));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(CritMultiplierProperty), JsonObject->GetNumberField("CritMultiplier"));
			static UProperty* HasteProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Haste));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(HasteProperty), JsonObject->GetNumberField("Haste"));
			static UProperty* SpellPowerProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, SpellPower));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(SpellPowerProperty), JsonObject->GetNumberField("SpellPower"));
			static UProperty* SpellPenetrationProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, SpellPenetration));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(SpellPenetrationProperty), JsonObject->GetNumberField("SpellPenetration"));
			static UProperty* DefenseProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Defense));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(DefenseProperty), JsonObject->GetNumberField("Defense"));
			static UProperty* DodgeProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Dodge));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(DodgeProperty), JsonObject->GetNumberField("Dodge"));
			static UProperty* ParryProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Parry));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ParryProperty), JsonObject->GetNumberField("Parry"));
			static UProperty* AvoidanceProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Avoidance));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(AvoidanceProperty), JsonObject->GetNumberField("Avoidance"));
			static UProperty* VersatilityProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Versatility));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(VersatilityProperty), JsonObject->GetNumberField("Versatility"));
			static UProperty* MultishotProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Multishot));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MultishotProperty), JsonObject->GetNumberField("Multishot"));
			static UProperty* InitiativeProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Initiative));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(InitiativeProperty), JsonObject->GetNumberField("Initiative"));
			static UProperty* NaturalArmorProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, NaturalArmor));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(NaturalArmorProperty), JsonObject->GetNumberField("NaturalArmor"));
			static UProperty* PhysicalArmorProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, PhysicalArmor));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(PhysicalArmorProperty), JsonObject->GetNumberField("PhysicalArmor"));
			static UProperty* BonusArmorProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, BonusArmor));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(BonusArmorProperty), JsonObject->GetNumberField("BonusArmor"));
			static UProperty* ForceArmorProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, ForceArmor));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ForceArmorProperty), JsonObject->GetNumberField("ForceArmor"));
			static UProperty* MagicArmorProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, MagicArmor));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(MagicArmorProperty), JsonObject->GetNumberField("MagicArmor"));
			static UProperty* ResistanceProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Resistance));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ResistanceProperty), JsonObject->GetNumberField("Resistance"));
			static UProperty* ReloadSpeedProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, ReloadSpeed));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(ReloadSpeedProperty), JsonObject->GetNumberField("ReloadSpeed"));
			static UProperty* RangeProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Range));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(RangeProperty), JsonObject->GetNumberField("Range"));
			static UProperty* SpeedProperty = FindFieldChecked<UProperty>(UOWSAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UOWSAttributeSet, Speed));
			GetAbilitySystemComponent()->SetNumericAttributeBase(FGameplayAttribute(SpeedProperty), JsonObject->GetNumberField("Speed"));

			/*
			Speed = JsonObject->GetNumberField("Speed");
			Initiative = JsonObject->GetNumberField("Initiative");
			NaturalArmor = JsonObject->GetNumberField("NaturalArmor");
			Resistance = JsonObject->GetNumberField("Resistance");
			TeamNumber = JsonObject->GetNumberField("TeamNumber");
			Perception = JsonObject->GetNumberField("Perception");
			Acrobatics = JsonObject->GetNumberField("Acrobatics");
			Climb = JsonObject->GetNumberField("Climb");
			Stealth = JsonObject->GetNumberField("Stealth");
			Thirst = JsonObject->GetNumberField("Thirst");
			Hunger = JsonObject->GetNumberField("Hunger");

			Gold = JsonObject->GetNumberField("Gold");
			Silver = JsonObject->GetNumberField("Silver");
			Copper = JsonObject->GetNumberField("Copper");
			FreeCurrency = JsonObject->GetNumberField("FreeCurrency");
			PremiumCurrency = JsonObject->GetNumberField("PremiumCurrency");
			Score = JsonObject->GetNumberField("Score");
			*/

			//LoadCustomCharacterStats();
			//UpdateCharacterStatsAfterLoading();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnGetCharacterStatsResponseReceived Server returned no data!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnGetCharacterStatsResponseReceived Error accessing server!"));
	}
}


void AOWSCharacterWithAbilities::UpdateCharacterStats()
{
	AOWSPlayerController* PC = Cast<AOWSPlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		PlayerName = PlayerName.Replace(TEXT(" "), TEXT("%20"));

		TSharedRef<IHttpRequest> Request = Http->CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AOWSCharacterWithAbilities::OnUpdateCharacterStatsResponseReceived);
		//This is the url on which to process the request
		FString url = FString(TEXT("http://" + RPGAPIPath + "/RPGServer/UpdateCharacterStats"));

		FString PostParameters = FString(TEXT("id=")) + PlayerName 
			+ FString(TEXT("&CharacterLevel=")) + FString::FromInt(this->CharacterLevel)
			+ FString(TEXT("&Gender=")) + FString::FromInt(this->Gender)
			+ FString(TEXT("&Weight=")) + "0"
			+ FString(TEXT("&Size=")) + "0"
			+ FString(TEXT("&Fame=")) + "0"
			+ FString(TEXT("&Alignment=")) + "0"
			+ FString(TEXT("&Description=")) + ""
			+ FString(TEXT("&XP=")) + FString::FromInt(this->XP)
			+ FString(TEXT("&X=")) + FString::SanitizeFloat(this->GetActorLocation().X)
			+ FString(TEXT("&Y=")) + FString::SanitizeFloat(this->GetActorLocation().Y)
			+ FString(TEXT("&Z=")) + FString::SanitizeFloat(this->GetActorLocation().Z)
			+ FString(TEXT("&RX=")) + FString::SanitizeFloat(this->GetActorRotation().Roll)
			+ FString(TEXT("&RY=")) + FString::SanitizeFloat(this->GetActorRotation().Pitch)
			+ FString(TEXT("&RZ=")) + FString::SanitizeFloat(this->GetActorRotation().Yaw)
			+ FString(TEXT("&TeamNumber=")) + FString::FromInt(this->TeamNumber)
			+ FString(TEXT("&HitDice=")) + FString::FromInt(this->HitDice)
			+ FString(TEXT("&Wounds=")) + FString::SanitizeFloat(this->Wounds)
			+ FString(TEXT("&Thirst=")) + FString::SanitizeFloat(this->Thirst)
			+ FString(TEXT("&Hunger=")) + FString::SanitizeFloat(this->Hunger)
			+ FString(TEXT("&MaxHealth=")) + FString::SanitizeFloat(OWSAttributes->MaxHealth)
			+ FString(TEXT("&Health=")) + FString::SanitizeFloat(OWSAttributes->Health)
			+ FString(TEXT("&HealthRegenRate=")) + FString::SanitizeFloat(OWSAttributes->HealthRegenRate)
			+ FString(TEXT("&MaxMana=")) + FString::SanitizeFloat(OWSAttributes->MaxMana)
			+ FString(TEXT("&Mana=")) + FString::SanitizeFloat(OWSAttributes->Mana)
			+ FString(TEXT("&ManaRegenRate=")) + FString::SanitizeFloat(OWSAttributes->ManaRegenRate)
			+ FString(TEXT("&MaxEnergy=")) + FString::SanitizeFloat(OWSAttributes->MaxEnergy)
			+ FString(TEXT("&Energy=")) + FString::SanitizeFloat(OWSAttributes->Energy)
			+ FString(TEXT("&EnergyRegenRate=")) + FString::SanitizeFloat(OWSAttributes->EnergyRegenRate)
			+ FString(TEXT("&MaxFatigue=")) + FString::SanitizeFloat(OWSAttributes->MaxFatigue)
			+ FString(TEXT("&Fatigue=")) + FString::SanitizeFloat(OWSAttributes->Fatigue)
			+ FString(TEXT("&FatigueRegenRate=")) + FString::SanitizeFloat(OWSAttributes->FatigueRegenRate)
			+ FString(TEXT("&MaxStamina=")) + FString::SanitizeFloat(OWSAttributes->MaxStamina)
			+ FString(TEXT("&Stamina=")) + FString::SanitizeFloat(OWSAttributes->Stamina)
			+ FString(TEXT("&StaminaRegenRate=")) + FString::SanitizeFloat(OWSAttributes->StaminaRegenRate)
			+ FString(TEXT("&MaxEndurance=")) + FString::SanitizeFloat(OWSAttributes->MaxEndurance)
			+ FString(TEXT("&Endurance=")) + FString::SanitizeFloat(OWSAttributes->Endurance)
			+ FString(TEXT("&EnduranceRegenRate=")) + FString::SanitizeFloat(OWSAttributes->EnduranceRegenRate)
			+ FString(TEXT("&Strength=")) + FString::SanitizeFloat(OWSAttributes->Strength)
			+ FString(TEXT("&Dexterity=")) + FString::SanitizeFloat(OWSAttributes->Dexterity)
			+ FString(TEXT("&Constitution=")) + FString::SanitizeFloat(OWSAttributes->Constitution)
			+ FString(TEXT("&Intellect=")) + FString::SanitizeFloat(OWSAttributes->Intellect)
			+ FString(TEXT("&Wisdom=")) + FString::SanitizeFloat(OWSAttributes->Wisdom)
			+ FString(TEXT("&Charisma=")) + FString::SanitizeFloat(OWSAttributes->Charisma)
			+ FString(TEXT("&Agility=")) + FString::SanitizeFloat(OWSAttributes->Agility)
			+ FString(TEXT("&Spirit=")) + FString::SanitizeFloat(OWSAttributes->Spirit)
			+ FString(TEXT("&Magic=")) + FString::SanitizeFloat(OWSAttributes->Magic)
			+ FString(TEXT("&Fortitude=")) + FString::SanitizeFloat(OWSAttributes->Fortitude)
			+ FString(TEXT("&Reflex=")) + FString::SanitizeFloat(OWSAttributes->Reflex)
			+ FString(TEXT("&Willpower=")) + FString::SanitizeFloat(OWSAttributes->Willpower)
			+ FString(TEXT("&BaseAttack=")) + FString::SanitizeFloat(OWSAttributes->BaseAttack)
			+ FString(TEXT("&BaseAttackBonus=")) + FString::SanitizeFloat(OWSAttributes->BaseAttackBonus)
			+ FString(TEXT("&AttackPower=")) + FString::SanitizeFloat(OWSAttributes->AttackPower)
			+ FString(TEXT("&AttackSpeed=")) + FString::SanitizeFloat(OWSAttributes->AttackSpeed)
			+ FString(TEXT("&CritChance=")) + FString::SanitizeFloat(OWSAttributes->CritChance)
			+ FString(TEXT("&CritMultiplier=")) + FString::SanitizeFloat(OWSAttributes->CritMultiplier)
			+ FString(TEXT("&Haste=")) + FString::SanitizeFloat(OWSAttributes->Haste)
			+ FString(TEXT("&SpellPower=")) + FString::SanitizeFloat(OWSAttributes->SpellPower)
			+ FString(TEXT("&SpellPenetration=")) + FString::SanitizeFloat(OWSAttributes->SpellPenetration)
			+ FString(TEXT("&Defense=")) + FString::SanitizeFloat(OWSAttributes->Defense)
			+ FString(TEXT("&Dodge=")) + FString::SanitizeFloat(OWSAttributes->Dodge)
			+ FString(TEXT("&Parry=")) + FString::SanitizeFloat(OWSAttributes->Parry)
			+ FString(TEXT("&Avoidance=")) + FString::SanitizeFloat(OWSAttributes->Avoidance)
			+ FString(TEXT("&Versatility=")) + FString::SanitizeFloat(OWSAttributes->Versatility)
			+ FString(TEXT("&Multishot=")) + FString::SanitizeFloat(OWSAttributes->Multishot)
			+ FString(TEXT("&Initiative=")) + FString::SanitizeFloat(OWSAttributes->Initiative)
			+ FString(TEXT("&NaturalArmor=")) + FString::SanitizeFloat(OWSAttributes->NaturalArmor)
			+ FString(TEXT("&PhysicalArmor=")) + FString::SanitizeFloat(OWSAttributes->PhysicalArmor)
			+ FString(TEXT("&BonusArmor=")) + FString::SanitizeFloat(OWSAttributes->BonusArmor)
			+ FString(TEXT("&ForceArmor=")) + FString::SanitizeFloat(OWSAttributes->ForceArmor)
			+ FString(TEXT("&MagicArmor=")) + FString::SanitizeFloat(OWSAttributes->MagicArmor)
			+ FString(TEXT("&Resistance=")) + FString::SanitizeFloat(OWSAttributes->Resistance)
			+ FString(TEXT("&ReloadSpeed=")) + FString::SanitizeFloat(OWSAttributes->ReloadSpeed)
			+ FString(TEXT("&Range=")) + FString::SanitizeFloat(OWSAttributes->Range)
			+ FString(TEXT("&Speed=")) + FString::SanitizeFloat(OWSAttributes->Speed)
			+ FString(TEXT("&Gold=")) + FString::FromInt(this->Gold)
			+ FString(TEXT("&Silver=")) + FString::FromInt(this->Silver)
			+ FString(TEXT("&Copper=")) + FString::FromInt(this->Copper)
			+ FString(TEXT("&FreeCurrency=")) + FString::FromInt(this->FreeCurrency)
			+ FString(TEXT("&PremiumCurrency=")) + FString::FromInt(this->PremiumCurrency)
			+ FString(TEXT("&Perception=")) + FString::SanitizeFloat(this->Perception)
			+ FString(TEXT("&Acrobatics=")) + FString::SanitizeFloat(this->Acrobatics)
			+ FString(TEXT("&Climb=")) + FString::SanitizeFloat(this->Climb)
			+ FString(TEXT("&Stealth=")) + FString::SanitizeFloat(this->Stealth)
			+ FString(TEXT("&Score=")) + FString::FromInt(this->Score)
			+ FString(TEXT("&CustomerGUID=")) + RPGAPICustomerKey;
		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
		Request->SetContentAsString(PostParameters);
		Request->ProcessRequest();
	}
}

void AOWSCharacterWithAbilities::OnUpdateCharacterStatsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		/*if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{

		}
		else
		{
		UE_LOG(LogTemp, Error, TEXT("OnGetCharacterStatsResponseReceived Server returned no data!"));
		}*/
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnGetCharacterStatsResponseReceived Error accessing server!"));
	}
}

/*
TArray<FGameplayTag> AOWSCharacterWithAbilities::GetCombatHasIconTags()
{
	TArray<FGameplayTag> CombatHasIconTags;

	static FGameplayTagContainer OwnerTags;
	OwnerTags.Reset();
	GetAbilitySystemComponent()->GetOwnedGameplayTags(OwnerTags);

	OwnerTags.GetGameplayTagArray(CombatHasIconTags);

	return CombatHasIconTags;
}
*/
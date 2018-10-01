// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OWSCharacter.h"
#include "AbilitySystemInterface.h" 
#include "OWSAttributeSet.h"
//#include "OWSGameplayAbility.h"
#include "OWSCharacterWithAbilities.generated.h"


UENUM(BlueprintType)
enum class AbilityInput : uint8
{
	UseAbility1 UMETA(DisplayName = "UseAbility1"), //This maps the first ability(input ID should be 0 in int) to the action mapping(which you define in the project settings) by the name of "UseAbility1". "Use Spell 1" is the blueprint name of the element.
	UseAbility2 UMETA(DisplayName = "Use Ability 2"), //Maps ability 2(input ID 1) to action mapping UseAbility2. "Use Spell 2" is mostly used for when the enum is a blueprint variable.
	UseAbility3 UMETA(DisplayName = "Use Ability 3"),
	UseAbility4 UMETA(DisplayName = "Use Ability 4"),
	UseAbility5 UMETA(DisplayName = "Use Ability 5"),
	UseAbility6 UMETA(DisplayName = "Use Ability 6"),
	UseAbility7 UMETA(DisplayName = "Use Ability 7"),
	UseAbility8 UMETA(DisplayName = "Use Ability 8"),
	UseAbility9 UMETA(DisplayName = "Use Ability 9"),
	UseAbility0 UMETA(DisplayName = "Use Ability 0"),
	UseWeapon1 UMETA(DisplayName = "Use Weapon 1"),
	UseWeapon2 UMETA(DisplayName = "Use Weapon 2"),
};

/**
 * 
 */
UCLASS(abstract, BlueprintType, hidecategories = (CharacterStats, Health))
class OWSPLUGIN_API AOWSCharacterWithAbilities : public AOWSCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOWSCharacterWithAbilities();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_Controller() override;

	void AddDefaultGameplayAbilities();

	FTimerHandle OnGetCharacterStatsTimerHandle;

	UFUNCTION(BlueprintImplementableEvent, Category = "Damage")
		void CalculateUpdatedDamage(float OriginalDamage, const UOWSAttributeSet* SourceAttributes, FGameplayTagContainer EffectTags, float& NewDamage);

	//Get Character Stats
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void GetCharacterStats();

	void OnGetCharacterStatsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	//Update Character Stats
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void UpdateCharacterStats();

	void OnUpdateCharacterStatsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		UOWSAttributeSet* OWSAttributes;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystem;
	};

	UFUNCTION(BlueprintCallable, Category = Combat)
		void ChangeSpell(int SpellSlot, TSubclassOf<class UGameplayAbility> NewSpell);
	UFUNCTION(BlueprintCallable, Category = Combat)
		void GrantAbility(TSubclassOf<class UGameplayAbility> NewAbility);
	UFUNCTION(BlueprintCallable, Category = Combat)
		void GrantAbilityKeyBind(TSubclassOf<class UGameplayAbility> NewAbility, int AbilityLevel, int InputID);
	UFUNCTION(BlueprintImplementableEvent, Category = Combat)
		void OnDeath();

	//Spells
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spells)
		TArray<FGameplayAbilitySpecHandle> SpellAbilityHandles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> Ability1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> Ability2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> Ability3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> Ability4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> Ability5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> Ability6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> Ability7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> Ability8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> Ability9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> Ability0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> Weapon1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> Weapon2;
	
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		UOWSGameplayAbility* Ability9Ptr;*/
	
	/*
	UFUNCTION(BlueprintCallable, Category = "Abilities")
		TArray<FGameplayTag> GetCombatHasIconTags();
	*/



};

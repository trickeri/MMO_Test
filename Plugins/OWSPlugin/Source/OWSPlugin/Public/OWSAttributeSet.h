// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "OWSAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class OWSPLUGIN_API UOWSAttributeSet : public UAttributeSet
{
	GENERATED_UCLASS_BODY()
	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_HitDie, Category = "RPGAttributes")
		mutable float HitDie;
	UFUNCTION()
		void OnRep_HitDie() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, HitDie); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Wounds, Category = "RPGAttributes")
		mutable float Wounds;
	UFUNCTION()
		void OnRep_Wounds() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Wounds); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Thirst, Category = "RPGAttributes")
		mutable float Thirst;
	UFUNCTION()
		void OnRep_Thirst() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Thirst); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Hunger, Category = "RPGAttributes")
		mutable float Hunger;
	UFUNCTION()
		void OnRep_Hunger() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Hunger); }


	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MaxHealth, Category = "RPGAttributes")//, meta = (HideFromModifiers))			// You can't make a GameplayEffect modify Health Directly (go through)
		mutable float	MaxHealth;
	UFUNCTION()
		void OnRep_MaxHealth()
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, MaxHealth);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Health, Category = "RPGAttributes", meta = (HideFromModifiers))
		mutable float	Health;
	UFUNCTION() 
		void OnRep_Health() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Health); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_HealthRegenRate, Category = "RPGAttributes")
		mutable float HealthRegenRate;
	UFUNCTION()
		void OnRep_HealthRegenRate() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, HealthRegenRate); }


	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MaxMana, Category = "RPGAttributes")
		mutable float MaxMana;
	UFUNCTION()
		void OnRep_MaxMana() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, MaxMana); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Mana, Category = "RPGAttributes")
		mutable float Mana;
	UFUNCTION()
		void OnRep_Mana() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Mana); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ManaRegenRate, Category = "RPGAttributes")
		mutable float ManaRegenRate;
	UFUNCTION()
		void OnRep_ManaRegenRate() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, ManaRegenRate); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MaxEnergy, Category = "RPGAttributes")
		mutable float MaxEnergy;
	UFUNCTION()
		void OnRep_MaxEnergy() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, MaxEnergy); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Energy, Category = "RPGAttributes")
		mutable float Energy;
	UFUNCTION()
		void OnRep_Energy() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Energy); }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_EnergyRegenRate, Category = "RPGAttributes")
		mutable float EnergyRegenRate;
	UFUNCTION()
		void OnRep_EnergyRegenRate() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, EnergyRegenRate); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MaxFatigue, Category = "RPGAttributes")
		mutable float MaxFatigue;
	UFUNCTION()
		void OnRep_MaxFatigue() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, MaxFatigue); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Fatigue, Category = "RPGAttributes")
		mutable float Fatigue;
	UFUNCTION()
		void OnRep_Fatigue() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Fatigue); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_FatigueRegenRate, Category = "RPGAttributes")
		mutable float FatigueRegenRate;
	UFUNCTION()
		void OnRep_FatigueRegenRate() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, FatigueRegenRate); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MaxStamina, Category = "RPGAttributes")
		mutable float MaxStamina;
	UFUNCTION()
		void OnRep_MaxStamina() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, MaxStamina); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Stamina, Category = "RPGAttributes")
		mutable float Stamina;
	UFUNCTION()
		void OnRep_Stamina() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Stamina); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_StaminaRegenRate, Category = "RPGAttributes")
		mutable float StaminaRegenRate;
	UFUNCTION()
		void OnRep_StaminaRegenRate() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, StaminaRegenRate); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MaxEndurance, Category = "RPGAttributes")
		mutable float MaxEndurance;
	UFUNCTION()
		void OnRep_MaxEndurance() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, MaxEndurance); }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Endurance, Category = "RPGAttributes")
		mutable float Endurance;
	UFUNCTION()
		void OnRep_Endurance() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Endurance); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_EnduranceRegenRate, Category = "RPGAttributes")
		mutable float EnduranceRegenRate;
	UFUNCTION()
		void OnRep_EnduranceRegenRate() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, EnduranceRegenRate); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Strength, Category = "RPGAttributes")
		mutable float Strength;
	UFUNCTION()
		void OnRep_Strength() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Strength); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Dexterity, Category = "RPGAttributes")
		mutable float Dexterity;
	UFUNCTION()
		void OnRep_Dexterity() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Dexterity); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Constitution, Category = "RPGAttributes")
		mutable float Constitution;
	UFUNCTION()
		void OnRep_Constitution() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Constitution); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Intellect, Category = "RPGAttributes")
		mutable float Intellect;
	UFUNCTION()
		void OnRep_Intellect() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Intellect); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Wisdom, Category = "RPGAttributes")
		mutable float Wisdom;
	UFUNCTION()
		void OnRep_Wisdom() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Wisdom); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Charisma, Category = "RPGAttributes")
		mutable float Charisma;
	UFUNCTION()
		void OnRep_Charisma() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Charisma); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Agility, Category = "RPGAttributes")
		mutable float Agility;
	UFUNCTION()
		void OnRep_Agility() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Agility); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Spirit, Category = "RPGAttributes")
		mutable float Spirit;
	UFUNCTION()
		void OnRep_Spirit() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Spirit); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Magic, Category = "RPGAttributes")
		mutable float Magic;
	UFUNCTION()
		void OnRep_Magic() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Magic); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Fortitude, Category = "RPGAttributes")
		mutable float Fortitude;
	UFUNCTION()
		void OnRep_Fortitude() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Fortitude); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Reflex, Category = "RPGAttributes")
		mutable float Reflex;
	UFUNCTION()
		void OnRep_Reflex() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Reflex); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Willpower, Category = "RPGAttributes")
		mutable float Willpower;
	UFUNCTION()
		void OnRep_Willpower() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Willpower); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_BaseAttack, Category = "RPGAttributes")
		mutable float BaseAttack;
	UFUNCTION()
		void OnRep_BaseAttack() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, BaseAttack); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_BaseAttackBonus, Category = "RPGAttributes")
		mutable float BaseAttackBonus;
	UFUNCTION()
		void OnRep_BaseAttackBonus() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, BaseAttackBonus); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_AttackPower, Category = "RPGAttributes")
		mutable float AttackPower;
	UFUNCTION()
		void OnRep_AttackPower() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, AttackPower); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_AttackSpeed, Category = "RPGAttributes")
		mutable float AttackSpeed;
	UFUNCTION()
		void OnRep_AttackSpeed() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, AttackSpeed); }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_CritChance, Category = "RPGAttributes")
		mutable float CritChance;
	UFUNCTION()
		void OnRep_CritChance() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, CritChance); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_CritMultiplier, Category = "RPGAttributes")
		mutable float CritMultiplier;
	UFUNCTION()
		void OnRep_CritMultiplier() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, CritMultiplier); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Haste, Category = "RPGAttributes")
		mutable float Haste;
	UFUNCTION()
		void OnRep_Haste() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Haste); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_SpellPower, Category = "RPGAttributes")
		mutable float SpellPower;
	UFUNCTION()
		void OnRep_SpellPower() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, SpellPower); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_SpellPenetration, Category = "RPGAttributes")
		mutable float SpellPenetration;
	UFUNCTION()
		void OnRep_SpellPenetration() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, SpellPenetration); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Defense, Category = "RPGAttributes")
		mutable float Defense;
	UFUNCTION()
		void OnRep_Defense() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Defense); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Dodge, Category = "RPGAttributes")
		mutable float Dodge;
	UFUNCTION()
		void OnRep_Dodge() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Dodge); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Parry, Category = "RPGAttributes")
		mutable float Parry;
	UFUNCTION()
		void OnRep_Parry() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Parry); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Avoidance, Category = "RPGAttributes")
		mutable float Avoidance;
	UFUNCTION()
		void OnRep_Avoidance() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Avoidance); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Versatility, Category = "RPGAttributes")
		mutable float Versatility;
	UFUNCTION()
		void OnRep_Versatility() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Versatility); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Multishot, Category = "RPGAttributes")
		mutable float Multishot;
	UFUNCTION()
		void OnRep_Multishot() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Multishot); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Initiative, Category = "RPGAttributes")
		mutable float Initiative;
	UFUNCTION()
		void OnRep_Initiative() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Initiative); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_NaturalArmor, Category = "RPGAttributes")
		mutable float NaturalArmor;
	UFUNCTION()
		void OnRep_NaturalArmor() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, NaturalArmor); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_PhysicalArmor, Category = "RPGAttributes")
		mutable float PhysicalArmor;
	UFUNCTION()
		void OnRep_PhysicalArmor() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, PhysicalArmor); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_BonusArmor, Category = "RPGAttributes")
		mutable float BonusArmor;
	UFUNCTION()
		void OnRep_BonusArmor() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, BonusArmor); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ForceArmor, Category = "RPGAttributes")
		mutable float ForceArmor;
	UFUNCTION()
		void OnRep_ForceArmor() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, ForceArmor); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MagicArmor, Category = "RPGAttributes")
		mutable float MagicArmor;
	UFUNCTION()
		void OnRep_MagicArmor() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, MagicArmor); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Resistance, Category = "RPGAttributes")
		mutable float Resistance;
	UFUNCTION()
		void OnRep_Resistance() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Resistance); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ReloadSpeed, Category = "RPGAttributes")
		mutable float ReloadSpeed;
	UFUNCTION()
		void OnRep_ReloadSpeed() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, ReloadSpeed); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Range, Category = "RPGAttributes")
		mutable float Range;
	UFUNCTION()
		void OnRep_Range() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Range); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Speed, Category = "RPGAttributes")
		mutable float Speed;
	UFUNCTION()
		void OnRep_Speed() { GAMEPLAYATTRIBUTE_REPNOTIFY(UOWSAttributeSet, Speed); }

	/** This Damage is just used for applying negative health mods. Its not a 'persistent' attribute. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeTest", meta = (HideFromLevelInfos))		// You can't make a GameplayEffect 'powered' by Damage (Its transient)
		mutable float	Damage;

	/** This Healing is just used for applying positive health mods. Its not a 'persistent' attribute. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeTest", meta = (HideFromLevelInfos))		// You can't make a GameplayEffect 'powered' by Healing (Its transient)
		mutable float	Healing;

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData &Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
};

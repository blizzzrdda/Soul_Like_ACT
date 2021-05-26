// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SoulAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/** This holds all of the attributes used by abilities, it instantiates a copy of this on every character */
UCLASS()
class SOUL_LIKE_ACT_API USoulAttributeSet : public UAttributeSet
{
    GENERATED_BODY()
	
public:
    USoulAttributeSet();
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Health)
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Posture;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Posture)
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Shield)
	
#pragma region Defense
    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxHealth)
	UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData MaxHealth_Base;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxHealth_Base)
    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData MaxHealth_Inc;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxHealth_Inc)
    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData MaxHealth_More;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxHealth_More)

    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData MaxPosture;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxPosture)
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxPosture_Base;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxPosture_Base)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxPosture_Inc;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxPosture_Inc)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxPosture_More;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxPosture_More)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxShield)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxShield_Percent_Base;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxShield_Percent_Base)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxShield_Percent_Inc;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxShield_Percent_Inc)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxShield_Percent_More;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxShield_Percent_More)
	
    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData Tenacity;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, Tenacity)
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Tenacity_Base;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Tenacity_Base)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Tenacity_Inc;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Tenacity_Inc)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Tenacity_More;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Tenacity_More)

    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData Armour;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, Armour)
    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData Armour_Base;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, Armour_Base)
    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData Armour_Inc;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, Armour_Inc)
    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData Armour_More;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, Armour_More)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData AilmentResilience;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, AilmentResilience)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData AilmentResilience_Base;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, AilmentResilience_Base)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData AilmentResilience_Inc;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, AilmentResilience_Inc)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData AilmentResilience_More;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, AilmentResilience_More)
#pragma endregion
#pragma region Offense
    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData AttackPower;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, AttackPower)
    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData AttackPower_Base;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, AttackPower_Base)
    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData AttackPower_Inc;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, AttackPower_Inc)
    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData AttackPower_More;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, AttackPower_More)

    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData AttackSpeed;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, AttackSpeed)
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData AttackSpeed_Base;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, AttackSpeed_Base)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData AttackSpeed_Inc;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, AttackSpeed_Inc)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData AttackSpeed_More;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, AttackSpeed_More)

    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData PostureCrumble;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, PostureCrumble)
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData PostureCrumble_Base;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, PostureCrumble_Base)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData PostureCrumble_Inc;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, PostureCrumble_Inc)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData PostureCrumble_More;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, PostureCrumble_More)

    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData CriticalStrike;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, CriticalStrike)
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData CriticalStrike_Base;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, CriticalStrike_Base)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData CriticalStrike_Inc;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, CriticalStrike_Inc)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData CriticalStrike_More;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, CriticalStrike_More)

    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData CriticalMulti;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, CriticalMulti)
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData CriticalMulti_Base;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, CriticalMulti_Base)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData CriticalMulti_Inc;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, CriticalMulti_Inc)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData CriticalMulti_More;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, CriticalMulti_More)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData BleedingChance;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, BleedingChance)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData BleedingChance_Base;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, BleedingChance_Base)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData BleedingChance_Inc;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, BleedingChance_Inc)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData BleedingChance_More;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, BleedingChance_More)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData BleedingMulti;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, BleedingMulti)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData BleedingMulti_Base;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, BleedingMulti_Base)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData BleedingMulti_Inc;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, BleedingMulti_Inc)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData BleedingMulti_More;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, BleedingMulti_More)
#pragma endregion

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Leech;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Leech)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Leech_Base;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Leech_Base)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Leech_Inc;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Leech_Inc)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Leech_More;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Leech_More)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MoveSpeed)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MoveSpeed_Base;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MoveSpeed_Base)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MoveSpeed_Inc;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MoveSpeed_Inc)
    UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MoveSpeed_More;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MoveSpeed_More)

    UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos))
    FGameplayAttributeData Damage;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, Damage)
    UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos))
    FGameplayAttributeData PostureDamage;
    ATTRIBUTE_ACCESSORS(USoulAttributeSet, PostureDamage)


protected:
    void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
                                     const FGameplayAttributeData& MaxAttribute, float NewMaxValue,
                                     const FGameplayAttribute& AffectedAttributeProperty);
};

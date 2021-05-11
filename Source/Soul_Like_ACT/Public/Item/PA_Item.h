// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SoulAssetManager.h"
#include "Types/DA_DefaultAbilities.h"
#include "GameplayTags.h"
#include "Abilities/SoulAttributeSet.h"
#include "Types/PlayerMoveSet.h"
#include "PA_Item.generated.h"

class USoulGameplayAbility;

USTRUCT(BlueprintType)
struct SOUL_LIKE_ACT_API FSoulModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Modifier)
	TSubclassOf<UModifierAbility> Modifier_CDO;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Modifier)
	uint8 Level;


	bool operator==(const FSoulModifier& Other) const;

	FSoulModifier& CombineModifier(const FSoulModifier& Other);

	bool IsValid() const;
};

inline bool FSoulModifier::operator==(const FSoulModifier& Other) const
{
	return Modifier_CDO == Other.Modifier_CDO;
}

inline FSoulModifier& FSoulModifier::CombineModifier(const FSoulModifier& Other)
{
	if (Other.IsValid() && *this == Other)
	{
		this->Level += Other.Level;
	}
	return *this;
}

inline bool FSoulModifier::IsValid() const
{
	return Modifier_CDO && Level > 0;
}

// 0-7
UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	InvalidEquipmentSlot = 8,
	Head = 4,
	Body = 5,
	Gloves = 6,
	Legs = 3,
	Boots = 7,
	Amulet = 1,
	Ring = 2,
	Weapon = 0
};

UENUM(BlueprintType)
enum class EJewelSocketStatus : uint8
{
	InvalidJewelSocket,
	Empty,
	Inserted
};

/** Base class for all items, do not blueprint directly */
UCLASS(Abstract)
class SOUL_LIKE_ACT_API UPA_Item_Abstract : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	EEquipmentSlot EquipSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	UTexture2D* ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	TMap<FGameplayAttribute, float> Attributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Modifiers)
	TArray<FSoulModifier> ItemModifiers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Slots)
	int32 MaxJewelSockets;

	UFUNCTION(BlueprintCallable)
	virtual TMap<FString, float> GetItemAttributes() const { return TMap<FString, float>(); }

	inline virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(ItemType, GetFName());
	}
};

UCLASS(Blueprintable, BlueprintType)
class SOUL_LIKE_ACT_API UPA_Jewel : public UPA_Item_Abstract
{
	GENERATED_BODY()

public:
	UPA_Jewel()
	{
		ItemType = USoulAssetManager::JewelItemType;
		EquipSlot = EEquipmentSlot::InvalidEquipmentSlot;
	}

	virtual TMap<FString, float> GetItemAttributes() const override
	{
		return TMap<FString, float>();
	}
};

UCLASS(Blueprintable, BlueprintType)
class SOUL_LIKE_ACT_API UPA_Weapon : public UPA_Item_Abstract
{
	GENERATED_BODY()

public:
	UPA_Weapon()
		:DefaultStance(EStanceType::Middle)
	{
		ItemType = USoulAssetManager::WeaponItemType;
		EquipSlot = EEquipmentSlot::Weapon;

		Attributes.Add(USoulAttributeSet::GetAttackPower_BaseAttribute(), 1.f);
		Attributes.Add(USoulAttributeSet::GetAttackSpeed_BaseAttribute(), 1.f);
		Attributes.Add(USoulAttributeSet::GetPostureCrumble_BaseAttribute(), 1.f);
		Attributes.Add(USoulAttributeSet::GetCriticalStrike_BaseAttribute(), .05f);
		Attributes.Add(USoulAttributeSet::GetBleedingChance_BaseAttribute(), 0.f);
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapoon)
	UDA_DefaultAbilitiesForWeapon* DefaultAbilitiesForWeapon_PENDING;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapoon)
	float WeaponDamageMulti = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapoon)
	float WeaponPostureDamageMulti = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WorldMesh)
	EStanceType DefaultStance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WorldMesh)
	int32 HandleLength = 20;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WorldMesh)
	int32 BladeLength = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WorldMesh)
	class USkeletalMesh* WeaponMesh;
	
	virtual TMap<FString, float> GetItemAttributes() const override
	{
		TMap<FString, float> Result;
		Result.Add("Damage", Attributes[USoulAttributeSet::GetAttackPower_BaseAttribute()]);
		Result.Add("Speed", Attributes[USoulAttributeSet::GetAttackSpeed_BaseAttribute()]);
		Result.Add("Critical Strike", Attributes[USoulAttributeSet::GetPostureCrumble_BaseAttribute()]);
		Result.Add("Posture Crumble", Attributes[USoulAttributeSet::GetCriticalStrike_BaseAttribute()]);
		Result.Add("Bleeding Chance", Attributes[USoulAttributeSet::GetBleedingChance_BaseAttribute()]);
		return Result;
	}
};

UCLASS(Blueprintable, BlueprintType)
class SOUL_LIKE_ACT_API UPA_Armour : public UPA_Item_Abstract
{
	GENERATED_BODY()

public:
	UPA_Armour()
	{
		ItemType = USoulAssetManager::ArmourItemType;
		EquipSlot = EEquipmentSlot::Body;

		Attributes.Add(USoulAttributeSet::GetArmour_BaseAttribute(), 1.f);
		Attributes.Add(USoulAttributeSet::GetTenacity_BaseAttribute(), 0.f);
		Attributes.Add(USoulAttributeSet::GetAilmentResilience_BaseAttribute(), 0.f);
	}

	virtual TMap<FString, float> GetItemAttributes() const override
	{
		TMap<FString, float> Result;

		Result.Add("Armour", Attributes[USoulAttributeSet::GetArmour_BaseAttribute()]);
		Result.Add("Tenacity", Attributes[USoulAttributeSet::GetTenacity_BaseAttribute()]);
		Result.Add("Ailment Resilience",
				   Attributes[USoulAttributeSet::GetAilmentResilience_BaseAttribute()]);
		return Result;
	}
};

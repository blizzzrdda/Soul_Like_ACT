#pragma once
#include "Abilities/SoulGameplayAbility.h"
#include "Engine/DataAsset.h"
#include "DA_DefaultAbilities.generated.h"

UCLASS()
class SOUL_LIKE_ACT_API UDA_DefaultAbilitiesForWeapon : public UDataAsset
{
	GENERATED_BODY()

public:
	UDA_DefaultAbilitiesForWeapon()
	{
	}

	UPROPERTY(EditDefaultsOnly, Category = ActiveAbilities)
	TArray<TSubclassOf<UActiveAbility>> ActiveAbilityClasses;
};

UCLASS()
class SOUL_LIKE_ACT_API UDA_DefaultAbilties : public UDataAsset
{
	GENERATED_BODY()

	public:
	UDA_DefaultAbilties()
	{
	}

	UPROPERTY(EditDefaultsOnly, Category = ActiveAbilities)
	TArray<TSubclassOf<UGameplayAbility>> AbilityClasses;
};

UCLASS()
class SOUL_LIKE_ACT_API UDA_DefaultEffects : public UDataAsset
{
	GENERATED_BODY()

public:
	UDA_DefaultEffects()
	{
	}

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> AggregatorEffects;
};

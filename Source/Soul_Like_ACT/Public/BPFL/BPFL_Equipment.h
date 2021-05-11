// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/PlayerMoveSet.h"
#include "BPFL_Equipment.generated.h"

UENUM(BlueprintType)
enum class ESoundSelector_MeleeAttack : uint8
{
	Attacking,
	HitTarget,
	Post
};

UCLASS()
class SOUL_LIKE_ACT_API UBPFL_Equipment : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Equipment)
	static FGameplayTag GetWeaponHitTag()
	{
		return FGameplayTag::RequestGameplayTag("Event.Montage.Shared.WeaponHit");
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Equipment)
	static FGameplayTag GetWeaponAttackSoundTag(ESoundSelector_MeleeAttack SoundSelector);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Equipment)
	static bool StanceToSocketName(EStanceType Stance, FName& OutSocketName);
};

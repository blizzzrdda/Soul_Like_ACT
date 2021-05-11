// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Soul_Like_ACT.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "BPFL_GameplayTag.generated.h"


UENUM(BlueprintType)
enum class EParryStatus : uint8
{
    Unguard,
    Normal,
    Perfect,
};

UCLASS()
class SOUL_LIKE_ACT_API UBPFL_GameplayTag : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = Tag)
    static void PrintTagContainer(const FGameplayTagContainer& TagContainer)
    {
        LOG_FUNC_SUCCESS(TagContainer.ToString());
    }

    UFUNCTION(BlueprintCallable, Category = Tag)
    static void IsCharacterParry(const UAbilitySystemComponent* ASC, EParryStatus& ParryResult);
};

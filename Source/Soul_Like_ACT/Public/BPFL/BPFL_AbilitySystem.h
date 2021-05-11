// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/SoulAttributeSet.h"
#include "Abilities/SoulGameplayAbility.h"
#include "BPFL_AbilitySystem.generated.h"

UCLASS(meta = (ScriptName = "AbilitySystemLibrary"))
class SOUL_LIKE_ACT_API UBPFL_AbilitySystem : public UAbilitySystemBlueprintLibrary
{
	GENERATED_BODY()

public:
	UBPFL_AbilitySystem();
	


	
	UFUNCTION(BlueprintCallable,Category = Ability, Meta = (Tooltip =
    "This function can be used to create the event data through weapon on hit"))
	static void CreateEventData(const AActor* Target, const AActor* Source, const FHitResult& InpHitResult,
                            const FGameplayTag EventTag, const float EventMagnitude,
                            FGameplayEventData& OutpEventData);

	UFUNCTION(BlueprintCallable,Category = Ability, Meta = (Tooltip =
    "This function can be used to create the event data through weapon on hit"))
    static void CreateEventDataWithoutHitResult(const AActor* Target, const AActor* Source,
                            const FGameplayTag EventTag, const float EventMagnitude,
                            FGameplayEventData& OutpEventData);

	UFUNCTION(BlueprintCallable, Category = GameplayTag)
    static bool OverrideActorGameplayTag(UAbilitySystemComponent* AbilitySysComp, const FGameplayTag& InTag, bool bAdd);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = GameplayTag)
    static bool DoesActorHasTag(UAbilitySystemComponent* AbilitySysComp, FGameplayTag InTag);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GameplayTag)
    static bool DoesActorHasAnyTags(UAbilitySystemComponent* AbilitySysComp, FGameplayTagContainer InTagContainer);
    
    static UGameplayEffectUIData* GetActiveGameplayEffectUIData(FActiveGameplayEffectHandle Handle);

}; 
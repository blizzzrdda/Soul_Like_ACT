// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AbilitySystemComponent.h"
#include "SoulGameplayAbility.h"
#include "SoulAbilitySystemComponent.generated.h"

UCLASS()
class SOUL_LIKE_ACT_API USoulAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()
public:
    /** Returns a list of currently active ability instances that match the tags */
    void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer,
                                    TArray<USoulGameplayAbility*>& ActiveAbilities) const;

    /** Returns the default level used for ability activations, derived from the character */
    int32 GetDefaultAbilityLevel() const;

    /** Version of function in AbilitySystemGlobals that returns correct type */
    UFUNCTION(BlueprintCallable)
    static USoulAbilitySystemComponent* GetAbilitySystemComponentFromActor(
        const AActor* Actor, bool LookForComponent = false);

    //------------------------------GameplayCue---------------------------------------
    UFUNCTION(BlueprintCallable, Category = GameplayCue)
    void ExecuteGameplayCue_BP(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters)
    {
        ExecuteGameplayCue(GameplayCueTag, GameplayCueParameters);
    }
    //--------------------------------------------------------------------------------

    
    UFUNCTION(BlueprintCallable)
    static FActiveGameplayEffectHandle ApplyEffectToSelf(const AActor* Actor,
                                                      const TSubclassOf<UGameplayEffect> GameplayEffect,
                                                      const int32 AbilityLevel);

    UFUNCTION(BlueprintCallable)
    static FActiveGameplayEffectHandle ApplyEffectToTarget(const AActor* FromActor, const AActor* TargetActor,
                                                        const TSubclassOf<UGameplayEffect> GameplayEffect,
                                                        const int32 AbilityLevel);

    bool TryActivateAbilityByClassWithDelegate(FGameplayAbilitySpecHandle& OutAbilitySpecHandle
                                               , TSubclassOf<UGameplayAbility> InAbilityToActivate
                                               , FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate =
                                                   nullptr);

    bool TryActivateAbilityWithDelegate(FGameplayAbilitySpecHandle AbilityToActivate, bool bAllowRemoteActivation,
                                        FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate = nullptr);

    UFUNCTION(BlueprintCallable, Category = Attributes)
    void AddAttributeValue(const FGameplayAttribute Attribute, float AdditiveValue, bool bClampNegative = false);

    UFUNCTION(BlueprintCallable, Category = GameplayAbility)
    bool IsAbilityGiven(TSubclassOf<UGameplayAbility> Ability);
};

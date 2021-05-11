// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Abilities/SoulAbilityTypes.h"
#include "SoulTargetType.generated.h"

class ASoulCharacterBase;
class AActor;
struct FGameplayEventData;

/**
 * Class that is used to determine targeting for abilities
 * It is meant to be blueprinted to run target logic
 * This does not subclass GameplayAbilityTargetActor because this class is never instanced into the world
 * This can be used as a basis for a game-specific targeting blueprint
 * If your targeting is more complicated you may need to instance into the world once or as a pooled actor
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class SOUL_LIKE_ACT_API USoulTargetType : public UObject
{
    GENERATED_BODY()

public:
    // Constructor and overrides
    USoulTargetType()
    {
    }

    /** Called to determine targets to apply gameplay effects to */
    UFUNCTION(BlueprintNativeEvent)
    void GetTargets(ASoulCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData,
                    TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

/** Trivial target type that uses the owner */
UCLASS(NotBlueprintable)
class SOUL_LIKE_ACT_API USoulTargetType_UseOwner : public USoulTargetType
{
    GENERATED_BODY()

public:
    // Constructor and overrides
    USoulTargetType_UseOwner()
    {
    }

    /** Uses the passed in event data */
    virtual void GetTargets_Implementation(ASoulCharacterBase* TargetingCharacter, AActor* TargetingActor,
                                           FGameplayEventData EventData, TArray<FHitResult>& OutHitResults,
                                           TArray<AActor*>& OutActors) const override;
};

/** Trivial target type that pulls the target out of the event data */
UCLASS(NotBlueprintable)
class SOUL_LIKE_ACT_API USoulTargetType_UseEventData : public USoulTargetType
{
    GENERATED_BODY()

public:
    // Constructor and overrides
    USoulTargetType_UseEventData()
    {
    }

    /** Uses the passed in event data */
    virtual void GetTargets_Implementation(ASoulCharacterBase* TargetingCharacter, AActor* TargetingActor,
                                           FGameplayEventData EventData, TArray<FHitResult>& OutHitResults,
                                           TArray<AActor*>& OutActors) const override;
};
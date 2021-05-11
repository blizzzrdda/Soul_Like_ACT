// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/SoulTargetType.h"
#include "Abilities/SoulGameplayAbility.h"
#include "SoulCharacterBase.h"

void USoulTargetType::GetTargets_Implementation(ASoulCharacterBase* TargetingCharacter, AActor* TargetingActor,
                                                FGameplayEventData EventData, TArray<FHitResult>& OutHitResults,
                                                TArray<AActor*>& OutActors) const
{
    return;
}

void USoulTargetType_UseOwner::GetTargets_Implementation(ASoulCharacterBase* TargetingCharacter, AActor* TargetingActor,
                                                         FGameplayEventData EventData,
                                                         TArray<FHitResult>& OutHitResults,
                                                         TArray<AActor*>& OutActors) const
{
    OutActors.Add(TargetingCharacter);
}

void USoulTargetType_UseEventData::GetTargets_Implementation(ASoulCharacterBase* TargetingCharacter,
                                                             AActor* TargetingActor, FGameplayEventData EventData,
                                                             TArray<FHitResult>& OutHitResults,
                                                             TArray<AActor*>& OutActors) const
{
    const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
    if (FoundHitResult)
    {
        OutHitResults.Add(*FoundHitResult);
    }
    else if (EventData.Target)
    {
        OutActors.Add(const_cast<AActor*>(EventData.Target));
    }
}

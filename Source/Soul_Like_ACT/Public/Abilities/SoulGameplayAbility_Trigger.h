// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SoulGameplayAbility.h"
#include "SoulGameplayAbility_Trigger.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API USoulGameplayAbility_Trigger : public USoulGameplayAbility
{
	GENERATED_BODY()

	USoulGameplayAbility_Trigger();

	// USE K2 INSTEAD
	// virtual bool ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* Payload) const override;
};

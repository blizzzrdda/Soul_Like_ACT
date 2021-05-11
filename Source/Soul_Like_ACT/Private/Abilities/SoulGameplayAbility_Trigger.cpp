// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/SoulGameplayAbility_Trigger.h"

USoulGameplayAbility_Trigger::USoulGameplayAbility_Trigger()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true;
}

// bool USoulGameplayAbility_Trigger::ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo* ActorInfo,
// 	const FGameplayEventData* Payload) const
// {
// 	if(Payload->OptionalObject->IsA(USoulJsonObjectWrapper::StaticClass()))
// 	{
// 		return true;
// 	}
//
// 	return false;
// }
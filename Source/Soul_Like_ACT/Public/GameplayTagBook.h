#pragma once
#include "GameplayTagContainer.h"

namespace NGameplayTagBook
{
	FGameplayTag ParryPerfect = FGameplayTag::RequestGameplayTag(FName{"Buffer.Parry.Perfect"}, true);
	FGameplayTag ParryPerNormal = FGameplayTag::RequestGameplayTag(FName{"Buffer.Parry.Normal"}, true);
	FGameplayTag Dodge = FGameplayTag::RequestGameplayTag(FName{"Buffer.Dodge"}, true);

	FGameplayTag DamageInstant = FGameplayTag::RequestGameplayTag(FName{"Damage.Duration.Instant"}, true);
	FGameplayTag DamageDot = FGameplayTag::RequestGameplayTag(FName{"Damage.Duration.Dot"}, true);
	FGameplayTag DamageCritical = FGameplayTag::RequestGameplayTag(FName{"Damage.Critical"}, true);

	FGameplayTag ReactionStun = FGameplayTag::RequestGameplayTag(FName{"Reaction.Stun"}, true);
}

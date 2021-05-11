// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/SoulAttributeSet.h"
#include "SoulCharacterBase.h"
#include "GameplayEffectExtension.h"

USoulAttributeSet::USoulAttributeSet()
	: Health(1.f)
	  , Posture(0.f)
	  , Shield(0.f)
	  , MaxHealth(1.f), MaxHealth_Base(1.f), MaxHealth_Inc(0.f)
	  , MaxHealth_More(1.f)
	  , MaxPosture(1.f)
	  , MaxPosture_Base(1.f), MaxPosture_Inc(0.f), MaxPosture_More(1.f)
	  , MaxShield_Percent_Base(0.f), MaxShield_Percent_Inc(0.f), MaxShield_Percent_More(1.f)
	  , Tenacity(0.0f)
	  , Tenacity_Base(0.f), Tenacity_Inc(0.f), Tenacity_More(1.f)
	  , Armour(0.f)
	  , Armour_Base(0.f), Armour_Inc(0.f), Armour_More(1.f)
	  , AilmentResilience(0.f)
	  , AilmentResilience_Base(0.f), AilmentResilience_Inc(0.f), AilmentResilience_More(1.f)
	  , AttackPower(0.0f)
	  , AttackPower_Base(0.f), AttackPower_Inc(0.f), AttackPower_More(1.f)
	  , AttackSpeed(1.0f)
	  , AttackSpeed_Base(1.f), AttackSpeed_Inc(0.f), AttackSpeed_More(1.f)
	  , PostureCrumble(0.0f)
	  , PostureCrumble_Base(1.f), PostureCrumble_Inc(0.f), PostureCrumble_More(1.f)
	  , CriticalStrike(0.0f)
	  , CriticalStrike_Base(.0f), CriticalStrike_Inc(0.f), CriticalStrike_More(1.f)
	  , CriticalMulti(0.5f)
	  , CriticalMulti_Base(.5f), CriticalMulti_Inc(0.f), CriticalMulti_More(1.f)
	  , BleedingChance(0.f)
	  , BleedingChance_Base(0.f), BleedingChance_Inc(0.f), BleedingChance_More(1.f)
	  , BleedingMulti(0.f)
	  , BleedingMulti_Base(0.f), BleedingMulti_Inc(0.f), BleedingMulti_More(1.f)
	  , Leech(0.0f)
	  , Leech_Base(0.f), Leech_Inc(0.f), Leech_More(1.f)
	  , MoveSpeed(400.0f)
	  , MoveSpeed_Base(1.f), MoveSpeed_Inc(0.f), MoveSpeed_More(1.f)
	  , Damage(0.0f)
	  , PostureDamage(0.0f)
{
}

void USoulAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
													const FGameplayAttributeData& MaxAttribute, float NewMaxValue,
													const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();

	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();

	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		const float NewDelta = (CurrentMaxValue > 0.f)
								   ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue
								   : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void USoulAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// This is called whenever attributes change, so for max health/mana we want to scale the current totals to match
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
}

void USoulAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	//const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	//const FGameplayTagContainer& TargetTags = *Data.EffectSpec.CapturedTargetTags.GetAggregatedTags();
	const FGameplayTagContainer& EffectSpecTags = Data.EffectSpec.DynamicAssetTags;


	// Compute the delta between old and new, if it is available
	float DeltaValue = 0;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		// If this was additive, store the raw delta value to be passed along later
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	ASoulCharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ASoulCharacterBase>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Get the Source actor
		AActor* SourceActor = nullptr;
		AController* SourceController = nullptr;
		ASoulCharacterBase* SourceCharacter = nullptr;
		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
			SourceController = Source->AbilityActorInfo->PlayerController.Get();
			if (SourceController == nullptr && SourceActor != nullptr)
			{
				if (APawn* Pawn = Cast<APawn>(SourceActor))
				{
					SourceController = Pawn->GetController();
				}
			}

			// Use the controller to find the source pawn
			if (SourceController)
			{
				SourceCharacter = Cast<ASoulCharacterBase>(SourceController->GetPawn());
			}
			else
			{
				SourceCharacter = Cast<ASoulCharacterBase>(SourceActor);
			}

			// Set the causer actor based on context if it's set
			if (Context.GetEffectCauser())
			{
				SourceActor = Context.GetEffectCauser();
			}
		}

		// Try to extract a hit result
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		/**
		 * Data.EffectSpec == FGameplayEffectSpec
		 * Data.EffectSpec.DynamicAssetTags
		 */
		bool bIsCritic = Data.EffectSpec.DynamicAssetTags.HasTagExact(
																	  FGameplayTag::RequestGameplayTag(FName{
																		  "Damage.Critical"
																	  }, true));

		bool bIsStun = Data.EffectSpec.DynamicAssetTags.HasTagExact(
																	FGameplayTag::RequestGameplayTag(FName{
																		"Damage.Stun"
																	}, true));

		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0)
		{
			// Apply the health change and then clamp it
			const float OldHealth = GetHealth();
			SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

			//On-Kill Proc
			if (GetHealth() <= 0.f)
			{
				(Cast<ASoulCharacterBase>(SourceActor))->
					Notify_OnMeleeKill(SourceActor, TargetActor, HitResult);

				if (OldHealth > 0.f)
					TargetCharacter->HandleOnDead(HitResult, EffectSpecTags, SourceCharacter,
												  SourceActor);
			}
		}

		if (TargetCharacter)
		{
			FGameplayTagContainer LocalContainer;
			Data.EffectSpec.GetAllAssetTags(LocalContainer);

			//Dot damage
			if (LocalContainer.HasTagExact(FGameplayTag::RequestGameplayTag(FName{"Damage.Dot"}, true)))
				TargetCharacter->HandleDotDamage(LocalDamageDone, bIsCritic, bIsStun, HitResult, EffectSpecTags,
												 SourceCharacter, SourceActor);
				//Hit damage
			else
				TargetCharacter->HandleDamage(LocalDamageDone, bIsCritic, bIsStun, HitResult, EffectSpecTags,
											  SourceCharacter,
											  SourceActor);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetPostureDamageAttribute())
	{
		// Get the Source actor
		AActor* SourceActor = nullptr;
		AController* SourceController = nullptr;
		ASoulCharacterBase* SourceCharacter = nullptr;
		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
			SourceController = Source->AbilityActorInfo->PlayerController.Get();
			if (SourceController == nullptr && SourceActor != nullptr)
			{
				if (APawn* Pawn = Cast<APawn>(SourceActor))
				{
					SourceController = Pawn->GetController();
				}
			}

			// Use the controller to find the source pawn
			if (SourceController)
			{
				SourceCharacter = Cast<ASoulCharacterBase>(SourceController->GetPawn());
			}
			else
			{
				SourceCharacter = Cast<ASoulCharacterBase>(SourceActor);
			}

			// Set the causer actor based on context if it's set
			if (Context.GetEffectCauser())
			{
				SourceActor = Context.GetEffectCauser();
			}
		}

		// Try to extract a hit result
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		/**
		 * Data.EffectSpec == FGameplayEffectSpec
		 * Data.EffectSpec.DynamicAssetTags
		 */
		bool bIsCritic = Data.EffectSpec.DynamicAssetTags.HasTagExact(
																	  FGameplayTag::RequestGameplayTag(FName{
																		  "Damage.Critical"
																	  }, true));

		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalPostureDamageDone = GetPostureDamage();
		SetPostureDamage(0.f);

		if (LocalPostureDamageDone > 0)
		{
			// Apply the health change and then clamp it
			const float OldPosture = GetPosture();
			SetPosture(FMath::Clamp(OldPosture + LocalPostureDamageDone, 0.0f, GetMaxPosture()));

			if (TargetCharacter)
			{
				// This is proper damage
				TargetCharacter->HandlePostureDamage(LocalPostureDamageDone, bIsCritic, HitResult, EffectSpecTags,
													 SourceCharacter, SourceActor);

				if (!FMath::IsNearlyEqual(OldPosture, GetPosture(), .1f) && FMath::IsNearlyEqual(GetPosture(),
					GetMaxPosture(), .1f))
				{
					TargetCharacter->HandleOnCrumble(LocalPostureDamageDone, bIsCritic, HitResult, EffectSpecTags,
													 SourceCharacter, SourceActor);
				}
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Handle other health changes such as from healing or direct modifiers
		// First clamp it
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetPostureAttribute())
	{
		SetPosture(FMath::Clamp(GetPosture(), 0.0f, GetMaxPosture()));
	}
	// else if (Data.EvaluatedData.Attribute == GetLeechAttribute())
	// {
	//     SetLeech(FMath::Clamp(GetLeech(), 0.0f, 100.0f));
	// }
	// else if (Data.EvaluatedData.Attribute == GetAttackSpeedAttribute())
	// {
	//     SetLeech(FMath::Clamp(GetAttackSpeed(), 0.0f, 999.0f));
	// }
	// else if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
	// {
	//     SetMoveSpeed(FMath::Clamp(GetMoveSpeed(), 0.0f, 2000.0f));
	// }
	//     //Posture Strength
	// else if (Data.EvaluatedData.Attribute == GetTenacityAttribute())
	// {
	//     SetTenacity(FMath::Clamp(GetTenacity(), 0.0f, 9999.0f));
	// }
	//     else if (Data.EvaluatedData.Attribute == GetArmourAttribute())
	// {
	//     SetArmour(FMath::Clamp(GetArmour(), 0.0f, 9999.0f));
	// }
	// else if (Data.EvaluatedData.Attribute == GetAttackPowerAttribute())
	// {
	//     SetAttackPower(FMath::Clamp(GetAttackPower(), 0.0f, 9999.0f));
	// }
	// else if (Data.EvaluatedData.Attribute == GetPostureCrumbleAttribute())
	// {
	//     SetPostureCrumble(FMath::Clamp(GetPostureCrumble(), 0.0f, 9999.0f));
	// }
	// else if (Data.EvaluatedData.Attribute == GetCriticalStrikeAttribute())
	// {
	//     SetCriticalStrike(FMath::Clamp(GetCriticalStrike(), -999.f, 999.f));
	// }
	// else if (Data.EvaluatedData.Attribute == GetCriticalMultiAttribute())
	// {
	//     SetCriticalMulti(FMath::Clamp(GetCriticalMulti(), -999.f, 999.f));
	// }
}

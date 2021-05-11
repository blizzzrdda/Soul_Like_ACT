// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Abilities/GameplayAbility.h"
#include "Abilities/SoulAbilityTypes.h"
#include "GameplayTagContainer.h"
#include "AbilityTasks/AT_PlayMontageAndWaitForEvent.h"
#include "SoulJsonObject.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Types/PlayerMoveSet.h"

#include "SoulGameplayAbility.generated.h"

UCLASS(Abstract)
class SOUL_LIKE_ACT_API USoulGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	USoulGameplayAbility()
	{
	}

	/** Map of gameplay tags to gameplay effect containers */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayEffects)
	TMap<FGameplayTag, FSoulGameplayEffectContainer> EffectContainerMap;


	/** Applies a gameplay effect container, by creating and then applying the spec */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag ContainerTag,
																	 const FGameplayEventData& EventData,
																	 int32 OverrideGameplayLevel = -1);
protected:
	/** Make gameplay effect container spec to be applied later, using the passed in container */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FSoulGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(
		const FSoulGameplayEffectContainer& Container, const FGameplayEventData& EventData,
		int32 OverrideGameplayLevel = -1);

	/** Search for and make a gameplay effect container spec to be applied later, from the EffectContainerMap */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FSoulGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag,
																	 const FGameplayEventData& EventData,
																	 int32 OverrideGameplayLevel = -1);

	/** Applies a gameplay effect container spec that was Zpreviously created */
	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(
		const FSoulGameplayEffectContainerSpec& ContainerSpec);

	/**
	 * @brief By default, 1st optional object is *USoulJsonObjectWrapper*
	 * Make sure the EvenData contains DamageExecution and relative tags
	 * e.g. "Type.Damage.Physical"
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category = Damage)
	bool ApplyDamageEffectSpec(FGameplayTag EventTag, const FGameplayEventData& EventData);
};

UCLASS(Abstract)
class SOUL_LIKE_ACT_API UActiveAbility : public USoulGameplayAbility
{
	GENERATED_BODY()

	FName LatentSectionName;
	UPROPERTY()
	FTimerHandle JumpSectionHandle;
	
	UPROPERTY()
	AGameplayAbilityTargetActor* TA_Instance;

public:
	UActiveAbility();

	UFUNCTION(BlueprintCallable, meta = (DeprecatedFunction), Category = Ability)
	void DEPRECATED_EndLatentAbility(bool bUseDuration, FName InLatentSectionName, float Duration);

	UFUNCTION()
	void TryEndLatentAbility();

	/**
	 * Get the current instance or
	 * Generate a instance if there is no instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "TA_Class"), Category = Ability)
	AGameplayAbilityTargetActor* GetTargetActorInstance(TSubclassOf<AGameplayAbilityTargetActor> TA_Class);
};

UCLASS(Abstract)
class SOUL_LIKE_ACT_API UModifierAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UModifierAbility()
	{
		InstancingPolicy = EGameplayAbilityInstancingPolicy::Type::InstancedPerActor;
	}

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = ModifierInfo)
	FName ModifierName;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = ModifierInfo)
	TArray<FText> LevelDescription;

	UFUNCTION(BlueprintCallable, Category = ModifierInfo)
	int32 GetMaxLevel() const { return LevelDescription.Num(); }

	UPROPERTY(EditDefaultsOnly, Category=ModifierInfo)
	TArray<TSubclassOf<UGameplayEffect>> ModifierEffects;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> EffectHandles;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
							bool bWasCancelled) override;
};

UCLASS(Abstract)
class SOUL_LIKE_ACT_API UGA_Melee : public UActiveAbility
{
	GENERATED_BODY()

public:
	UGA_Melee()
	{
		ActivationBlockedTags.AddTagFast(FGameplayTag::RequestGameplayTag("Effect.Ailment.Dead", true));
		ActivationBlockedTags.AddTagFast(FGameplayTag::RequestGameplayTag("Effect.Ailment.Stun", true));
	}

protected:
	UPROPERTY(BlueprintReadOnly, Category = Animation)
	int32 NextComboIndex;
	UPROPERTY(BlueprintReadOnly, Category = Animation)
	int32 NextSectionIndex;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Animation)
	EAttackType NextAttackType;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Animation)
	FAttackCombo ComboMontages;

	/**
	 * @brief 
	 * @param bPlayNextCombo 
	 * @param ToAttackType 
	 * @param bPlayNextSection 
	 */
	UFUNCTION(BlueprintCallable, Category = Animation)
	void SetNextCombo(bool bPlayNextCombo, EAttackType ToAttackType, bool bPlayNextSection);

	/**
	 * For channeling attack
	 */
	UFUNCTION(BlueprintCallable, Category = Animation)
	bool SetLastSection();

	UFUNCTION(BlueprintCallable, Category = Animation)
	bool GetMontageToPlay(UAnimMontage*& MontageToPlay, FName& SectionToPlay);

	UFUNCTION(BlueprintCallable, Category = Animation)
	void ResetCombo()
	{
		NextComboIndex = 0;
		NextSectionIndex = 0;
		NextAttackType = EAttackType::Light;
	}

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
							bool bWasCancelled) override;
};

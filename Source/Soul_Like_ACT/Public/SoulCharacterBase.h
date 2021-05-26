// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/SoulAbilitySystemComponent.h"
#include "Abilities/SoulAttributeSet.h"
#include "Interfaces/Targetable.h"
#include "Item/WeaponActor.h"
#include "SoulCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTrigger_OnMeleeAttack, AActor*, SourceActor, AActor*, TargetActor,
											   const FHitResult, HitResult);

UENUM(BlueprintType)
enum class EIsControllerValid : uint8
{
	IsValid,
	IsNotValid,
};

UENUM(BlueprintType)
enum class EActorFaction : uint8
{
	Untargetable,
	Player,
	Enemy
};

UENUM(BlueprintType)
enum class ESoulMovementMode : uint8
{
	Idle,
	Walk,
	Run,
	Sprint
};

UCLASS(Abstract, NotBlueprintable)
class SOUL_LIKE_ACT_API ASoulCharacterBase : public ACharacter, public ITargetable, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASoulCharacterBase();

protected:
	ESoulMovementMode MovementMode = ESoulMovementMode::Run;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* TargetIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USoulModifierManager* ModifierManager;

	/** The component used to handle ability system interactions */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USoulAbilitySystemComponent* AbilitySystemComponent;

	/** List of attributes modified by the ability system */
	UPROPERTY()
	USoulAttributeSet* AttributeSet;

	FTimerHandle Handle_SlowMotion, Handler_SlowMotionDelay;

	void WaitForDilationReset()
	{
		CustomTimeDilation = 1.f;
	}

	void TriggerSlowMotion()
	{
		CustomTimeDilation = 0.003f;
		GetWorldTimerManager().SetTimer(Handle_SlowMotion, this, &ASoulCharacterBase::WaitForDilationReset, 1.f, false,
										0.2f);
	}

public:
	UFUNCTION()
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	USoulModifierManager* GetModifierManager() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EActorFaction Faction;

	static bool IsInRivalFaction(ASoulCharacterBase* DamageDealer, ASoulCharacterBase* DamageReceiver);

	UFUNCTION(BlueprintCallable)
	virtual bool IsTargetable() const override
	{
		return (Faction != EActorFaction::Untargetable
			&& !GetIsDead());
	}

	UFUNCTION(BlueprintCallable)
	virtual void ToggleLockIcon() override;

	UFUNCTION(BlueprintCallable)
	virtual bool IsAlive() const { return AttributeSet->GetHealth() > 0.f; }

	//Called by WeaponActor and OnHit
	UFUNCTION(BlueprintCallable)
	void TriggerSlowMotion_WithDelay(float Delay);

	void Notify_OnMeleeAttack(AActor* TargetActor, const FHitResult HitResult)
	{
		if (OnMeleeAttack.IsBound())
			OnMeleeAttack.Broadcast(this, TargetActor, HitResult);
	}

	void Notify_OnMeleeKill(AActor* SourceActor, AActor* TargetActor, const FHitResult HitResult)
	{
		if (OnMeleeKill.IsBound())
			OnMeleeKill.Broadcast(SourceActor, TargetActor, HitResult);
	}

	/** Returns the character level that is passed to the ability system */
	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const { return 1; }

	UFUNCTION(BlueprintCallable)
	bool GetIsDead() const
	{
		return AbilitySystemComponent->
			HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Effect.Ailment.Dead", true));;
	}

	UFUNCTION(BlueprintCallable)
	bool GetIsStun() const
	{
		return AbilitySystemComponent->
			HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Effect.Ailment.Stun", true));
	}

	UFUNCTION(BlueprintCallable)
	bool GetIsHealthZero() const { return FMath::IsNearlyEqual(AttributeSet->GetHealth(), 0.f); }

	UFUNCTION(BlueprintCallable)
	bool GetIsPostureFull() const
	{
		return FMath::IsNearlyEqual(AttributeSet->GetPosture(), AttributeSet->GetMaxPosture());
	}

protected:
	/**
	 * Called when character takes damage, which may have killed them
	 *
	 * @param DamageAmount Amount of damage that was done, not clamped based on current health
	 * @param HitInfo The hit info that generated this damage
	 * @param DamageTags The gameplay tags of the event that did the damage
	 * @param InstigatorCharacter The character that initiated this damage
	 * @param DamageCauser The actual actor that did the damage, might be a weapon or projectile
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const bool IsCriticaled, const bool bIsStun, const FHitResult& HitInfo,
				   const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter,
				   AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDotDamaged(float DamageAmount, const bool IsCriticaled, const bool bIsStun, const FHitResult& HitInfo,
					  const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter,
					  AActor* DamageCauser);

	/**
	*Called when character takes posture damage
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnPostureDamaged(float PostureDamageAmount, const bool IsCriticaled, const FHitResult& HitInfo,
						  const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter,
						  AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnDead(const FHitResult& HitInfo,
				   const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter,
				   AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnCrumbled(float PostureDamageAmount, const bool IsCriticaled, const FHitResult& HitInfo,
					   const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter,
					   AActor* DamageCauser);


#pragma region GameplayEffect_Delegate
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGameplayEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec,
									FActiveGameplayEffectHandle EffectHandle);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGameplayEffectRemoved(const FActiveGameplayEffect& EffectRemovalInfo);
#pragma endregion

	UPROPERTY(BlueprintAssignable)
	FTrigger_OnMeleeAttack OnMeleeAttack;

	UPROPERTY(BlueprintAssignable)
	FTrigger_OnMeleeAttack OnMeleeKill;

	// Called from RPGAttributeSet, these call BP events above
	virtual void HandleDamage(float DamageAmount, const bool IsCriticaled, const bool bIsStun,
							  const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
							  ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleDotDamage(float DamageAmount, const bool IsCriticaled, const bool bIsStun,
								 const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
								 ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleOnDead(const FHitResult& HitInfo,
							  const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter,
							  AActor* DamageCauser);

	virtual void HandlePostureDamage(float PostureDamageAmount, const bool IsCriticaled, const FHitResult& HitInfo,
									 const struct FGameplayTagContainer& DamageTags,
									 ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	virtual void HandleOnCrumble(float PostureDamageAmount, const bool IsCriticaled, const FHitResult& HitInfo,
								 const struct FGameplayTagContainer& DamageTags,
								 ASoulCharacterBase* InstigatorCharacter,
								 AActor* DamageCauser);

	UPROPERTY()
	TArray<ASoulCharacterBase*> CounterTargets;
	UPROPERTY()
	bool bCounterEnabled;

	virtual void ForceOverrideFacingDirection(float Alpha) { return; }

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual AWeaponActor* GetWeaponActor() { return nullptr; }

	UFUNCTION(BlueprintCallable, category = Movement)
	ESoulMovementMode GetMovementMode() const { return MovementMode; }

	UFUNCTION(BlueprintCallable, Category = Movement)
	float GetMovementModeMultiplier() const
	{
		switch (MovementMode)
		{
			case ESoulMovementMode::Walk: return .25f;
			case ESoulMovementMode::Run: return .5f;
			case ESoulMovementMode::Sprint: return 1.f;
			default: return 0.f;
		}
	}

	UFUNCTION(BlueprintCallable, Category = Movement)
	void SetMovementMode(ESoulMovementMode InMovementMode)
	{
		MovementMode = InMovementMode;
	}

	UFUNCTION(BlueprintCallable, Category = Direction)
	void BP_ForceOverrideFacingDirection(float Speed) { return ForceOverrideFacingDirection(Speed); }

	FVector BodySweep_ForwardVec;
	
	float SweepingSpeed;
	
	/**
	 * Call the function in ANS_BodySweeping
	 * If bUseTarget and Target is valid, then it sweeps to the target.
	 * Otherwise, it sweeps to the forward.
	 */
	UFUNCTION(BlueprintCallable)
	bool BodySweep_Init(const AActor* Target, bool bUseTarget, float InSweepingSpeed);
	UFUNCTION(BlueprintCallable)
	void BodySweep_Tick(float Delta);
	UFUNCTION(BlueprintCallable)
	void BodySweep_Finished();

	friend USoulAttributeSet;
};

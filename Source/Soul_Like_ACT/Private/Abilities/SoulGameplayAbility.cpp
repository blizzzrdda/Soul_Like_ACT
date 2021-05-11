// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/SoulGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/SoulAbilitySystemComponent.h"
#include "Abilities/SoulTargetType.h"
#include "SoulCharacterBase.h"
#include "Soul_Like_ACT.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"


TArray<FActiveGameplayEffectHandle> USoulGameplayAbility::ApplyEffectContainer(FGameplayTag ContainerTag,
																			   const FGameplayEventData& EventData,
																			   int32 OverrideGameplayLevel)
{
	const FSoulGameplayEffectContainerSpec Spec = MakeEffectContainerSpec(
																		  ContainerTag, EventData,
																		  OverrideGameplayLevel);
	return ApplyEffectContainerSpec(Spec);
}

FSoulGameplayEffectContainerSpec USoulGameplayAbility::MakeEffectContainerSpecFromContainer(
	const FSoulGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	// First figure out our actor info
	FSoulGameplayEffectContainerSpec ReturnSpec;
	AActor* OwningActor = GetOwningActorFromActorInfo();
	ASoulCharacterBase* OwningCharacter = Cast<ASoulCharacterBase>(OwningActor);
	USoulAbilitySystemComponent* OwningASC = USoulAbilitySystemComponent::
		GetAbilitySystemComponentFromActor(OwningActor);

	if (OwningASC)
	{
		// If we have a target type, run the targeting logic. This is optional, targets can be added later
		if (Container.TargetType.Get())
		{
			TArray<FHitResult> HitResults;
			TArray<AActor*> TargetActors;
			const USoulTargetType* TargetTypeCDO = Container.TargetType.GetDefaultObject();
			AActor* AvatarActor = GetAvatarActorFromActorInfo();
			TargetTypeCDO->GetTargets(OwningCharacter, AvatarActor, EventData, HitResults, TargetActors);
			ReturnSpec.AddTargets(HitResults, TargetActors);
		}

		// If we don't have an override level, use the default ont he ability system component
		if (OverrideGameplayLevel == INDEX_NONE)
		{
			OverrideGameplayLevel = OwningASC->GetDefaultAbilityLevel();
		}

		// Build GameplayEffectSpecs for each applied effect
		for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses)
		{
			FGameplayEffectSpecHandle newGEHandle = MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel);

			ReturnSpec.TargetGameplayEffectSpecs.
					   Add(newGEHandle);
		}
	}
	return ReturnSpec;
}

FSoulGameplayEffectContainerSpec USoulGameplayAbility::MakeEffectContainerSpec(
	FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FSoulGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag);

	if (FoundContainer)
	{
		return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}
	return FSoulGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> USoulGameplayAbility::ApplyEffectContainerSpec(
	const FSoulGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	// Iterate list of effect specs and apply them to their target data
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	}
	return AllEffects;
}

bool USoulGameplayAbility::ApplyDamageEffectSpec(FGameplayTag EventTag, const FGameplayEventData& EventData)
{
	//make effect container spec
	FSoulGameplayEffectContainerSpec EffectContainerSpec = MakeEffectContainerSpec(EventTag, EventData,
		GetAbilityLevel());

	//Get damge multi from json
	const USoulJsonObjectWrapper* JsonObj = Cast<USoulJsonObjectWrapper>(EventData.OptionalObject);

	float OutDamageMulti, OutCrumbleMulti, OutPostureMulti;
	bool IsJsonObjValid;
	JsonObj->JsonGetActionDamageMulties(OutDamageMulti, OutPostureMulti, OutCrumbleMulti, IsJsonObjValid);
	if (!IsJsonObjValid) return false;

	//Get Damage Effect
	const FGameplayEffectSpecHandle EffectSpecHandle = EffectContainerSpec.TargetGameplayEffectSpecs[0];
	if (!EffectSpecHandle.IsValid() || EffectSpecHandle.Data.Get()->Def->Executions.Num() == 0)
	{
		LOG_FUNC_FAILURE("EffectSpecHandle invalid");
		return false;
	}

	//Apply to SpecHandle
	FGameplayEffectSpec* Spec = EffectSpecHandle.Data.Get();
	if (Spec)
	{
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Type.Damage.Physical", false), OutDamageMulti);
		// LOG_FUNC_SUCCESS(FString::SanitizeFloat(OutDamageMulti))
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Type.Damage.Crumble", false), OutCrumbleMulti);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Type.Damage.Posture", false), OutPostureMulti);
	}

	//Apply efffect container
	ApplyEffectContainerSpec(EffectContainerSpec);
	return true;
}

UActiveAbility::UActiveAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTagFast(FGameplayTag::RequestGameplayTag("Effect.Ailment.Dead", true));
	ActivationBlockedTags.AddTagFast(FGameplayTag::RequestGameplayTag("Effect.Ailment.Stun", true));
}

void UActiveAbility::DEPRECATED_EndLatentAbility(bool bUseDuration, FName InLatentSectionName, float Duration)
{
	LatentSectionName = InLatentSectionName;

	if (bUseDuration && Duration > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(JumpSectionHandle
											   , this
											   , &UActiveAbility::TryEndLatentAbility
											   , Duration
											   , false,
											   -1);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green,
										 FString::SanitizeFloat(GetWorld()->GetTimerManager().
																			GetTimerRemaining(JumpSectionHandle)));
	}
	else
	{
		TryEndLatentAbility();
	}
}

void UActiveAbility::TryEndLatentAbility()
{
	if (!LatentSectionName.IsNone() && LatentSectionName.IsValid())
	{
		FGameplayEventData Payload;
		auto JumpSecPayLoad = USoulJsonObjectWrapper::MakeSoulJsonObject();
		JumpSecPayLoad->SetJumpSection(true, LatentSectionName.ToString());
		Payload.OptionalObject = JumpSecPayLoad;

		SendGameplayEvent(FGameplayTag::RequestGameplayTag("Ability.Event.Combo"), Payload);
	}
	else
	{
		K2_EndAbility();
	}
}

AGameplayAbilityTargetActor* UActiveAbility::GetTargetActorInstance(TSubclassOf<AGameplayAbilityTargetActor> TA_Class)
{
	if(TA_Instance && TA_Instance->IsA(TA_Class))
	{
		return TA_Instance;
	}
	else
	{
		TA_Instance = GetWorld()->SpawnActor<AGameplayAbilityTargetActor>(TA_Class);
		return TA_Instance;
	}
}

void UModifierAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
									   const FGameplayAbilityActorInfo* ActorInfo,
									   const FGameplayAbilityActivationInfo ActivationInfo,
									   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//init effects
	for (auto& ModifierClass : ModifierEffects)
	{
		EffectHandles.Add(USoulAbilitySystemComponent::ApplyEffectToSelf(
																		 GetOwningActorFromActorInfo(), ModifierClass,
																		 GetAbilityLevel())
						 );
	}
}

void UModifierAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								  const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
								  bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	//remove effects
	for (auto& EffectHandle : EffectHandles)
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(EffectHandle);
	}
	EffectHandles.Reset();
}

void UGA_Melee::SetNextCombo(bool bPlayNextCombo, EAttackType ToAttackType, bool bPlayNextSection)
{
	check(ComboMontages.IsValid());

	if (bPlayNextCombo)
	{
		NextComboIndex = NextComboIndex == ComboMontages.GetComboLength() - 1 ? 0 : NextComboIndex + 1;

		NextSectionIndex = 0;
	}

	//	If it's a new montage
	if (ToAttackType != NextAttackType)
	{
		NextSectionIndex = 0;
	}

	// Set next attack type
	NextAttackType = ToAttackType;

	UAnimMontage* CurrMontage;
	FName CurrSection;
	GetMontageToPlay(CurrMontage, CurrSection);

	const int32 tempNextSection = bPlayNextSection ? NextSectionIndex + 1 : 0;
	NextSectionIndex = CurrMontage->IsValidSectionIndex(tempNextSection) ? tempNextSection : 0;
}

bool UGA_Melee::SetLastSection()
{
	UAnimMontage* tempMontage;
	FName tempSectionName;

	GetMontageToPlay(tempMontage, tempSectionName);

	if (!tempMontage) return false;

	const auto LastSection = tempMontage->CompositeSections.Num() - 1;

	const bool IsSectionValid = LastSection != INDEX_NONE && LastSection != tempMontage->
		GetSectionIndex(tempSectionName);

	if (IsSectionValid) NextSectionIndex = LastSection;

	return IsSectionValid;
}

bool UGA_Melee::GetMontageToPlay(UAnimMontage*& MontageToPlay, FName& SectionToPlay)
{
	if (!ComboMontages.IsValid()) return false;

	auto& ComboSet = ComboMontages.GetComboSets()[NextComboIndex];

	ComboSet.GetMontage_Safe(NextAttackType, MontageToPlay);
	if (!MontageToPlay)
		return false;

	SectionToPlay = MontageToPlay->GetSectionName(NextSectionIndex);
	return true;
}

void UGA_Melee::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
						   const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
						   bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ResetCombo();
}

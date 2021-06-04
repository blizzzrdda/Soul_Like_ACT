// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/SoulModifierManager.h"
#include "SoulCharacterBase.h"
#include "Soul_Like_ACT.h"
#include "BPFL/BPFL_ItemMaker.h"
#include "BPFL/BPFL_Utilities.h"
#include "Player/EquipmentManager.h"

USoulModifierManager::USoulModifierManager()
{
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UDA_DefaultEffects>
		DefaultAggregators_Obj(TEXT("DA_DefaultAggregators'/Game/DataAssets/DefaultAbilities/DefaultAggregartors.DefaultAggregartors'"));
	if (DefaultAggregators_Obj.Succeeded())
		AttributeAggregators = DefaultAggregators_Obj.Object;
}

void USoulModifierManager::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->IsA(ASoulCharacterBase::StaticClass()))
	{
		SetupDefaultAbilities();
		LOG_FUNC_SUCCESS_AUTO();
	}
}

void USoulModifierManager::UpdateAggregators(USoulAbilitySystemComponent* ASC)
{
	if (!AttributeAggregators)
	{
		LOG_FUNC_ERROR("No Attribute Aggregators");
		return;
	}

	for (auto& Aggregator_GE : AttributeAggregators->AggregatorEffects)
	{
		ASC->ApplyEffectToSelf(GetOwner(), Aggregator_GE, 0);
		// LOG_FUNC_SUCCESS(Aggregator_GE->GetName());
	}
}

void USoulModifierManager::UpdateActiveAbilities()
{
	auto EM = GetOwner()->GetComponentByClass(UEquipmentManager::StaticClass());
	if (!EM) return;
	auto ASC = USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetOwner());
	if (!ASC) return;

	auto& Equipments = Cast<UEquipmentManager>(EM)->Equipments;

	for (auto& Gear : Equipments)
	{
		if (Gear.IsValid() && Gear.ItemBase->EquipSlot == EEquipmentSlot::Weapon)
		{
			const auto WeaponActiveAbility_DA =
				Cast<UPA_Weapon>(Gear.ItemBase)->DefaultAbilitiesForWeapon_PENDING;

			//Remove current abilities
			for (auto& ActivatedAbilityHandle : GrantedActiveAbilities)
			{
				LOG_FUNC_SUCCESS(ASC->FindAbilitySpecFromHandle(ActivatedAbilityHandle)->Ability->GetName() +
								 " canceled");
				ASC->CancelAbilityHandle(ActivatedAbilityHandle);
			}

			GrantedActiveAbilities.Reset();

			if (WeaponActiveAbility_DA && WeaponActiveAbility_DA->ActiveAbilityClasses.Num() != 0)
			{
				//Activate new abilities
				for (auto AbilityClass : WeaponActiveAbility_DA->ActiveAbilityClasses)
				{
					auto AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
					GrantedActiveAbilities.Add(ASC->GiveAbilityAndActivateOnce(AbilitySpec));
				}
			}
			else
			{
				LOG_FUNC_FAILURE("No abilities need to be initialized");
			}

			return;
		}
	}
}

void USoulModifierManager::UpdateModifiers()
{
	auto EM = GetOwner()->GetComponentByClass(UEquipmentManager::StaticClass());
	if(!EM)
		EM = GetWorld()->GetFirstPlayerController()->GetComponentByClass(UEquipmentManager::StaticClass());
	if (!EM)
	{
		LOG_FUNC_ERROR("Cannot find EquipmentManager");
		return;
	}

	auto ASC =
		USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetWorld()->GetFirstPlayerController()->
																		GetPawn());
	if (!ASC)
	{
		LOG_FUNC_ERROR("Cannot find ASC");
		return;
	}

	auto& Equipments = Cast<UEquipmentManager>(EM)->Equipments;

	TArray<FSoulModifier> ModifiersToInit;
	ModifiersToInit.Reserve(100);

	//Calculate updated modifiers
	for (auto& Gear : Equipments)
	{
		TArray<FSoulModifier> _garbage1;
		TArray<FSoulModifier> _garbage2;
		int32 EmptySlots;
		UBPFL_ItemMaker::GetModifiersOfItem(Gear, ModifiersToInit, _garbage1, _garbage2, EmptySlots);
	}


	//Cancel current ModifierAbilities
	for (auto AbilityHandle : GrantedModifiers)
		ASC->CancelAbilityHandle(AbilityHandle);

	GrantedModifiers.Reset();

	UBPFL_Utilities::MergeModifierArray(ModifiersToInit);
	//Init new modifiers
	for (auto NewModifier : ModifiersToInit)
	{
		auto AbilitySpec = FGameplayAbilitySpec(NewModifier.Modifier_CDO, NewModifier.Level);
		GrantedModifiers.Add(ASC->GiveAbilityAndActivateOnce(AbilitySpec));
	}

	if (OnModifiersUpdated.IsBound())
		OnModifiersUpdated.Broadcast(this);
}

void USoulModifierManager::ApplyItemStats(UPA_Item_Abstract* Item_PA, bool bAdded)
{
	USoulAbilitySystemComponent* ASC = USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetOwner());
	if (!ASC->IsValidLowLevel())
	{
		LOG_FUNC_ERROR("cannot find ASC");
		return;
	}
	if (!Item_PA)
	{
		LOG_FUNC_ERROR("Invalid item asset");
		return;
	}

	for (auto& Attribute : Item_PA->Attributes)
	{
		if (Attribute.Value > 0.f)
		{
			ASC->ApplyModToAttribute(Attribute.Key, EGameplayModOp::Additive,
									 bAdded ? Attribute.Value : -Attribute.Value);
		}
	}
}

void USoulModifierManager::InitStat(USoulAbilitySystemComponent* ASC)
{
	//init initial status
	if (InitStatEffect)
	{
		auto EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(GetOwner());
		ASC->ApplyGameplayEffectSpecToSelf(
										   FGameplayEffectSpec(
															   InitStatEffect.GetDefaultObject(),
															   EffectContextHandle,
															   1
															  )
										  );
	}
	else
	{
		LOG_FUNC_ERROR("Cant find InitStatEffect")
	}
}

void USoulModifierManager::UpdateDefaultAbilities(USoulAbilitySystemComponent* ASC)
{
	if (InitAbilities)
	{
		// Reactivate current default abilities
		for (auto& ActiveDefaultAbilityHandle : GrantedActiveAbilities)
		{
			ASC->CancelAbilityHandle(ActiveDefaultAbilityHandle);
		}

		GrantedActiveAbilities.Reset();

		// Apply new abilities
		for (auto& InitAbilityClass : InitAbilities->AbilityClasses)
		{
			auto AbilitySpec = FGameplayAbilitySpec(InitAbilityClass, 1, INDEX_NONE, GetOwner());

			// LOG_FUNC_SUCCESS(InitAbilityClass->GetDefaultObjectName().ToString())

			GrantedInitAbilities.Add(ASC->GiveAbilityAndActivateOnce(AbilitySpec));
		}
	}
	else
	{
		LOG_FUNC_ERROR("Cannot find InitAbilties")
	}
}

void USoulModifierManager::SetupDefaultAbilities()
{
	if (bAbilitiesInitialized)
	{
		LOG_FUNC_ERROR("was already called");
		return;
	}

	USoulAbilitySystemComponent* ASC = USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetOwner());

	if (!ASC->IsValidLowLevel())
	{
		LOG_FUNC_ERROR("cannot find ASC");
		return;
	}

	InitStat(ASC);

	UpdateAggregators(ASC);

	UpdateDefaultAbilities(ASC);

	bAbilitiesInitialized = true;
}

void USoulModifierManager::UpdateAbilitiesAndModifiers()
{
	UpdateActiveAbilities();
	UpdateModifiers();
}

void USoulModifierManager::GetActiveModifiers(TArray<UModifierAbility*>& Abilities) const
{
	Abilities.Reset();

	USoulAbilitySystemComponent* ASC = USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetOwner());
	if (!ASC->IsValidLowLevel())
	{
		LOG_FUNC_ERROR("cannot find ASC");
		return;
	}

	for (auto& Handle : GrantedModifiers)
	{
		const auto AbilitySpec = ASC->FindAbilitySpecFromHandle(Handle);
		Abilities.Add(static_cast<UModifierAbility*>(AbilitySpec->GetPrimaryInstance()));
	}
}

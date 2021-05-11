// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget_BuffPanel.h"
#include "GameplayEffectUIData.h"
#include "Soul_Like_ACT.h"
#include "Abilities/GameplayEffect_Buff.h"
#include "Abilities/SoulGameplayAbility.h"
#include "Abilities/UIData_Buff.h"

void UWidget_BuffSlot::SetBuffSlot(UAbilitySystemComponent* ASC, const FActiveGameplayEffectHandle& ActiveEffectHandle)
{
	const auto ActiveEff = ASC->GetActiveGameplayEffect(ActiveEffectHandle);
	const auto UIData = ActiveEff->Spec.Def->UIData;

	//Icon
	if(UIData && UIData->IsA(UUIData_Buff::StaticClass()))
	{
		BuffIcon->GetDynamicMaterial()->SetTextureParameterValue("Texture", Cast<UUIData_Buff>(UIData)->BuffIcon);
	}

	//Stacks
	int32 CurrStacks = ASC->GetCurrentStackCount(ActiveEffectHandle);
	if(CurrStacks == 1 || CurrStacks == 0)
	{
		BuffStacks->SetVisibility(ESlateVisibility::Hidden);
	}else
	{
		BuffStacks->SetVisibility(ESlateVisibility::HitTestInvisible);
		BuffStacks->SetText(FText::AsNumber(CurrStacks));
	}

	//Duration & Cooldown Scanner
	if(ActiveEff->Spec.Def->DurationPolicy == EGameplayEffectDurationType::HasDuration)
	{
		Duration = ActiveEff->GetDuration();
		RemainingDuration = Duration;
		//setup timer
		GetWorld()->GetTimerManager().SetTimer(CoolDownTickingHandle, this, &UWidget_BuffSlot::UpdateCoolDown, .1f, true);
	}
	else if(ActiveEff->Spec.Def->DurationPolicy == EGameplayEffectDurationType::Infinite)
	{
		BuffIcon->GetDynamicMaterial()->SetScalarParameterValue("Percent", 0.f);
	}
}

void UWidget_BuffSlot::UpdateCoolDown()
{
	BuffIcon->GetDynamicMaterial()->SetScalarParameterValue("Percent", 1.f - RemainingDuration/Duration);

	RemainingDuration -= .1f;

	if(RemainingDuration <= 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(CoolDownTickingHandle);	
	}
}

void UWidget_BuffPanel::BindOnEffectChanged(UAbilitySystemComponent* ASC)
{
	ASC->OnGameplayEffectAppliedDelegateToSelf.
                            AddUObject(this, &UWidget_BuffPanel::OnEffectApplied);
	ASC->OnAnyGameplayEffectRemovedDelegate().
                            AddUObject(this, &UWidget_BuffPanel::BP_OnGameplayEffectRemoved);
}

void UWidget_BuffPanel::OnEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec,
	FActiveGameplayEffectHandle EffectHandle)
{
	if(EffectSpec.Def->DurationPolicy == EGameplayEffectDurationType::Instant)
	{
		// LOG_FUNC_SUCCESS(EffectSpec.Def->GetClass()->GetName() + " is an instant effect");
		return;
	}

	if(!EffectSpec.Def->UIData)
	{
		// LOG_FUNC_SUCCESS(EffectSpec.Def->GetClass()->GetName() + " does not have UI Data"); 
		return;
	}

	if(!EffectSpec.Def->IsA(UGameplayEffect_Buff::StaticClass()))
	{
		// LOG_FUNC_SUCCESS(EffectSpec.Def->GetClass()->GetName() + " is not a Modifier GameplayEffect");
		return;
	}
	
	if(BuffSlotClass)
	{
		auto CurrWidget = BuffEffectSpecs.Find(EffectSpec.Def->GetClass());
		if(CurrWidget)
		{
			(*CurrWidget)->SetBuffSlot(ASC, EffectHandle);

			// LOG_FUNC_SUCCESS(EffectSpec.Def->GetName() + " updated");
		}
		else
		{
			UWidget_BuffSlot* NewBuffSlot = Cast<UWidget_BuffSlot>(CreateWidget(this, BuffSlotClass));
			NewBuffSlot->SetBuffSlot(ASC, EffectHandle);
			BuffBox->AddChildToHorizontalBox(NewBuffSlot);
			
			BuffEffectSpecs.Add(EffectSpec.Def->GetClass(), NewBuffSlot);

			// LOG_FUNC_SUCCESS(EffectSpec.Def->GetName() + " applied");
		}
	}
	else
	{
		LOG_FUNC_FAILURE("BuffSlotClass is not set");
	}
}

void UWidget_BuffPanel::BP_OnGameplayEffectRemoved(const FActiveGameplayEffect& EffectRemovalInfo)
{
	auto CurrWidget = BuffEffectSpecs.Find(EffectRemovalInfo.Spec.Def->GetClass());

	if(CurrWidget)
	{
		BuffEffectSpecs.Remove(EffectRemovalInfo.Spec.Def->GetClass());
		(*CurrWidget)->RemoveFromParent();

		LOG_FUNC_SUCCESS(EffectRemovalInfo.Spec.Def->GetName() + " is removed");
	}
	else
	{
		LOG_FUNC_FAILURE(EffectRemovalInfo.Spec.Def->GetName() + " cannot be found");
	}
}

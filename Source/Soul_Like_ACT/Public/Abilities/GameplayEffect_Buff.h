// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "UIData_Buff.h"
#include "GameplayEffect_Buff.generated.h"

UCLASS(Abstract)
class SOUL_LIKE_ACT_API UGameplayEffect_Buff : public UGameplayEffect
{
	GENERATED_BODY()

	protected:
	UGameplayEffect_Buff()
	{
		DurationPolicy = EGameplayEffectDurationType::HasDuration;
		UIData = NewObject<UUIData_Buff>();
	}
};

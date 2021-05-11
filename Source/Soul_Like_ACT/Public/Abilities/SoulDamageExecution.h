// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "SoulDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API USoulDamageExecution : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()

public:
    USoulDamageExecution();

    virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                        OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};

UCLASS()
class SOUL_LIKE_ACT_API USoulReflectionDamageExecution : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()

public:
    USoulReflectionDamageExecution();

    virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                        OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};

UCLASS()
class SOUL_LIKE_ACT_API USoulDotDamageExecution : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()

public:
    USoulDotDamageExecution();

    virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                        OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};

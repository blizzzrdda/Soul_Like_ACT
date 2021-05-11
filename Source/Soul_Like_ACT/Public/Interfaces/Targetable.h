// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Targetable.generated.h"

UENUM(BlueprintType)
enum class EActionStatus : uint8
{
    Idle,
    Attack,
    Dash,
    Block,
    Parry,
    Stun,
    Dead,
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTargetable : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class SOUL_LIKE_ACT_API ITargetable
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:

    virtual bool IsTargetable() const = 0;

    virtual void ToggleLockIcon() = 0;
};

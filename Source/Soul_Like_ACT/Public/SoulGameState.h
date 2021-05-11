// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameFramework/Actor.h"
#include "SoulGameState.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API ASoulGameState : public AGameState
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
};

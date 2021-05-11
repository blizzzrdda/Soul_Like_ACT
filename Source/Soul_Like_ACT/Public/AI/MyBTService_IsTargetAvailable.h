// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "MyBTService_IsTargetAvailable.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UMyBTService_IsTargetAvailable : public UBTService_BlackboardBase
{
    GENERATED_BODY()

    int32 counter;
public:
    UMyBTService_IsTargetAvailable();

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

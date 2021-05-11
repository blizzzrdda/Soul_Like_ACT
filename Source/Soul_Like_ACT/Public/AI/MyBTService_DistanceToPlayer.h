// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "MyBTService_DistanceToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UMyBTService_DistanceToPlayer : public UBTService
{
    GENERATED_BODY()
public:
    UMyBTService_DistanceToPlayer();

    UPROPERTY(EditAnywhere, Category = Blackboard)
    struct FBlackboardKeySelector TargetKey;
    UPROPERTY(EditAnywhere, Category = Blackboard)
    struct FBlackboardKeySelector DistanceKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyBTTaskNode_GetStrafeVector.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UMyBTTaskNode_GetStrafeVector : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UMyBTTaskNode_GetStrafeVector();
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FBlackboardKeySelector TargetKey;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float StrafeLength = 500.f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FBlackboardKeySelector StrafeVecKey;
    
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

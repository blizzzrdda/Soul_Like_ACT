// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Abilities/GameplayAbility.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_UseGA.generated.h"


UCLASS()
class SOUL_LIKE_ACT_API UBTT_UseGA : public UBTTaskNode
{
    GENERATED_UCLASS_BODY()

    /** starts this task, should return Succeeded, Failed or InProgress
     *  (use FinishLatentTask() when returning InProgress)
     * this function should be considered as const (don't modify state of object) if node is not instanced! */
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
    UPROPERTY(EditAnywhere, Category = Abilities)
    bool bUseKey;
    UPROPERTY(EditAnywhere, Category = Abilities)
    FBlackboardKeySelector GA_Melee_CDO;
    UPROPERTY(EditAnywhere, Category = Abilities)
    TSubclassOf<class UGameplayAbility> AbilityClass;

    UPROPERTY()
    UBehaviorTreeComponent* BTC;

    UFUNCTION()
    void OnGA_Ended(class UGameplayAbility* Ability);
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "BTT_UseGA_Duration.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UBTT_UseGA_Duration : public UBTTaskNode
{
	GENERATED_BODY()

    UPROPERTY()
    UBehaviorTreeComponent* BTC;
    UPROPERTY()
    FGameplayAbilitySpecHandle GA_Handle;
    
public:
    UBTT_UseGA_Duration();

    UPROPERTY(EditAnywhere, Category = Abilities)
    bool bUseKey;
    UPROPERTY(EditAnywhere, Category = Abilities)
    FBlackboardKeySelector GA_Melee_CDO;
    UPROPERTY(EditAnywhere, Category = Abilities)
    TSubclassOf<class UGameplayAbility> AbilityClass;
    UPROPERTY(EditAnywhere, Category = Abilities)
    float EndLatency;
    UPROPERTY(EditAnywhere, Category = Abilities)
    FName EndSection;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UFUNCTION()
    void OnGA_Ended(class UGameplayAbility* Ability);
};

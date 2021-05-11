// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SoulGameplayAbility.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RandomizeAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UBTT_RandomizeAttackAbility : public UBTTaskNode
{
private:
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector GA_Selector;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UGA_Melee>> MeleeAbilities;
		
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

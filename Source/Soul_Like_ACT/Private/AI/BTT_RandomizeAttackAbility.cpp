// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_RandomizeAttackAbility.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_RandomizeAttackAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TSubclassOf<UGA_Melee> RandomizedAbility = MeleeAbilities[FMath::RandRange(0, MeleeAbilities.Num() - 1)];

    OwnerComp.GetBlackboardComponent()->SetValueAsClass(GA_Selector.SelectedKeyName, RandomizedAbility);
    
    return EBTNodeResult::Succeeded;
}

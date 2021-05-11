// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/MyBTService_IsTargetAvailable.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "SoulCharacterBase.h"
#include "Mob/MobController.h"


UMyBTService_IsTargetAvailable::UMyBTService_IsTargetAvailable()
{
    bCreateNodeInstance = 1;
}


void UMyBTService_IsTargetAvailable::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


    AMobController* Owner = Cast<AMobController>(OwnerComp.GetOwner());
    if (!Owner)
    {
        //OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("SelfActor"), nullptr);
        Owner->GetBTComp()->StopTree(EBTStopMode::Forced);
        return;
    }
    ASoulCharacterBase* PlayerPawn = Cast<ASoulCharacterBase>(
        OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerPawn"));

    if (!PlayerPawn || PlayerPawn->GetAbilitySystemComponent()->GetNumericAttribute(USoulAttributeSet::GetHealthAttribute()) <= 0)
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
        return;
    }
    else
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
    }
}

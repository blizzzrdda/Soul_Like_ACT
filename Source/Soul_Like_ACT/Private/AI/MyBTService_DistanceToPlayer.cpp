// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/MyBTService_DistanceToPlayer.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Mob/MobController.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTService_DistanceToPlayer::UMyBTService_DistanceToPlayer()
{
    bCreateNodeInstance = 1;

    //bNotifyBecomeRelevant = 1;
}

void UMyBTService_DistanceToPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AActor* PlayeyPawn = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
    if (PlayeyPawn)
    {
        const float DistanceToPlayer = FVector::Distance(
            Cast<AMobController>(OwnerComp.GetOwner())->GetPawn()->GetActorLocation(), PlayeyPawn->GetActorLocation());
        OwnerComp.GetBlackboardComponent()->SetValueAsFloat(DistanceKey.SelectedKeyName, DistanceToPlayer);
        return;
    }
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat(DistanceKey.SelectedKeyName, -1.f);
}

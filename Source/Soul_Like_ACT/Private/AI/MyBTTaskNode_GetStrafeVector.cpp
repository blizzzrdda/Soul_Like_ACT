// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/MyBTTaskNode_GetStrafeVector.h"
#include "Mob/MobCharacter.h"
#include "GameFramework/Controller.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"


UMyBTTaskNode_GetStrafeVector::UMyBTTaskNode_GetStrafeVector()
{
    NodeName = "Find Strafe Vector On Nav Mesh";
}

EBTNodeResult::Type UMyBTTaskNode_GetStrafeVector::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
    AActor* SelfActor = Cast<AController>(OwnerComp.GetOwner())->GetPawn();

    FVector PlayerToMobVec;
    if(TargetActor)
        PlayerToMobVec = (TargetActor->GetActorLocation() - SelfActor->GetActorLocation()).GetSafeNormal2D(0.01f);
    else
        PlayerToMobVec = SelfActor->GetActorForwardVector().GetSafeNormal2D(0.01f);
    
    const FVector RightVecFromDistance = PlayerToMobVec.ToOrientationQuat().GetRightVector();
    const FVector ForwardVecFromDistance = PlayerToMobVec.GetSafeNormal();

    const FVector StrafeVec = SelfActor->GetActorLocation()
        + ForwardVecFromDistance * FMath::RandBool() * FMath::RandRange(StrafeLength * .35f, StrafeLength)
        + RightVecFromDistance * FMath::RandBool() * FMath::RandRange(StrafeLength * .35f, StrafeLength);

    FNavLocation StrafeVecOnNavMesh;
    FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld())->ProjectPointToNavigation(
        StrafeVec, StrafeVecOnNavMesh);

    OwnerComp.GetBlackboardComponent()->SetValueAsVector(StrafeVecKey.SelectedKeyName, StrafeVecOnNavMesh.Location);

    DrawDebugLine(GetWorld(), SelfActor->GetActorLocation(), StrafeVecOnNavMesh.Location, FColor::Blue, 0, 10.f, 0,
                  3.f);
    return EBTNodeResult::Succeeded;
}

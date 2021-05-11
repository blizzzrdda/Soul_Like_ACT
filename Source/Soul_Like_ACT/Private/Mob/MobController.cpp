// Fill out your copyright notice in the Description page of Project Settings.

#include "Mob/MobController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Mob/MobRageManager.h"
#include "Mob/MobCharacter.h"

AMobController::AMobController()
{
    //Perception
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
    SetPerceptionComponent(*AIPerceptionComponent);
    sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    sightConfig->SightRadius = 1500.f;
    sightConfig->LoseSightRadius = 2000.f;
    sightConfig->PeripheralVisionAngleDegrees = 360.0f;
    sightConfig->DetectionByAffiliation.bDetectEnemies = true;
    sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    AIPerceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());
    AIPerceptionComponent->ConfigureSense(*sightConfig);

    //BB and BT
    BlackBoardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBaordComponent"));
    BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

    RageComponent = CreateDefaultSubobject<UMobRageManager>("RageComponent");
}

void AMobController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    PossessedMob = Cast<AMobCharacter>(InPawn);

    AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMobController::AISenseUpdateMessage);

    //set up bb and bt
    check(BlockBoardData);
    check(BehaviorTreeAsset);
}

void AMobController::OnUnPossess()
{
    Super::OnUnPossess();

    PossessedMob = nullptr;
}

void AMobController::AISenseUpdateMessage(AActor* TargetActor, FAIStimulus Stimulus)
{
    if (!PossessedMob)
    {
        AIPerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &AMobController::AISenseUpdateMessage);
        return;
    }
    if(TargetActor && TargetActor->StaticClass()->IsChildOf(ASoulCharacterBase::StaticClass()))
    {
        if (Cast<ASoulCharacterBase>(TargetActor)->Faction == EActorFaction::Player)
        {
            if (Stimulus.WasSuccessfullySensed())
            {
                BlackBoardComp->SetValueAsObject("PlayerPawn", TargetActor);
                UE_LOG(LogTemp, Warning, TEXT("%s is founded by %s"), *TargetActor->GetName(), *PossessedMob->GetName());
            }
            else
            {
                BlackBoardComp->SetValueAsObject("PlayerPawn", nullptr);
                UE_LOG(LogTemp, Warning, TEXT("%s is lost by %s"), *TargetActor->GetName(), *PossessedMob->GetName());
            }
        }
    }
}

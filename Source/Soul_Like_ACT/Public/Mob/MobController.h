// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig.h"
#include "MobController.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API AMobController : public AAIController
{
    GENERATED_BODY()

protected:
    class AMobCharacter* PossessedMob;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = AI)
    class UAIPerceptionComponent* AIPerceptionComponent;

    UPROPERTY()
    UAISenseConfig_Sight* sightConfig;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = AI)
    class UBlackboardComponent* BlackBoardComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = AI)
    class UBehaviorTreeComponent* BehaviorTreeComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = AI)
    class UMobRageManager* RageComponent;

public:
    AMobController();
    
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIAssets)
    class UBlackboardData* BlockBoardData;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIAssets)
    class UBehaviorTree* BehaviorTreeAsset;

    UFUNCTION(BlueprintCallable)
    void AISenseUpdateMessage(AActor* TargetActor, FAIStimulus Stimulus);

    class UBehaviorTreeComponent* GetBTComp() const { return BehaviorTreeComp; }

    class UMobRageManager* GetRageManager() const { return RageComponent;}
};

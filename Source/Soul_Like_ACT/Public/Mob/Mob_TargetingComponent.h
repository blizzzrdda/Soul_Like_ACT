// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mob_TargetingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API UMob_TargetingComponent : public UActorComponent
{
    GENERATED_BODY()

protected:
    bool bIsLockingOn;

public:
    // Sets default values for this component's properties
    UMob_TargetingComponent();

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float InterpSpeed = 300.f;

protected:

    
    class AMobCharacter* OwnerRef;
    class AMobController* OwnerController;

    AActor* TargetPawn;
    bool bIsFacingTarget;
public:
    UFUNCTION(BlueprintCallable, Category = AI)
    void EnableTargeting(AActor* TargetActor);
    UFUNCTION(BlueprintCallable, Category = AI)
    void DisableTargeting();
    
    UFUNCTION(BlueprintCallable)
    bool GetIsEnabled() const { return bIsFacingTarget; }

    friend class AMobCharacter;
};

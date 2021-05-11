// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Soul_Like_ACT.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerSaveManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API UPlayerSaveManager : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UPlayerSaveManager();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPFL_Math.generated.h"

class AActor;

UENUM(BlueprintType)
enum class EFourDirection : uint8
{
    Forward,
    Backward,
    Left,
    Right
};

UCLASS()
class SOUL_LIKE_ACT_API UBPFL_Math : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = Math)
    static void FindDegreeToTarget(const AActor* HitActor, const AActor* Attacker, float& Result);
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = Math)
    static void FindDegreeToHitNorm(const FRotator& ActorRotation, const FVector& HitNorm, float& Result);

    /**
     * -180 - 180 degrees
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = Math)
    static void DegreeToFourDirection(float Degree, EFourDirection& Direction);

    /**
     * @return returns true if the input is not <180.f OR >180.f
     * The deviation of the yaw value based on the EFourDirection
     * e.g. when Deviation = 70, the character have to rotate by -20(70-90) degrees,
     * or when Deviation = 110, then rotate 20 (110-90)
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = Math)
    static bool GetDeviationToFourDirection(float Degree, float& Deviation);
};
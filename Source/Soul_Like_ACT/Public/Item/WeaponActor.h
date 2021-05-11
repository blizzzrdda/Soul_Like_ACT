// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PA_Item.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

class ASoulCharacterBase;

UCLASS(BlueprintType, NotBlueprintable)
class SOUL_LIKE_ACT_API AWeaponActor : public AActor
{
    GENERATED_BODY()

    UPROPERTY()
    class USkeletalMeshComponent* MeshComp;

public:
    AWeaponActor();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = 1))
    bool bCanDamageAllies;

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    UPA_Weapon* WeaponSpecs;

    float ActionDamageMulti;
    float ActionPostureDamageMulti;
    
    bool bAttackingSwitch;

    TArray<FVector> HitPoints_LastFrame;
    TArray<FVector> HitPOints_CurrFrame;

    UPROPERTY()
    TArray<AActor*> MyTargets;
public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void SetupWeaponSpecs(UPA_Weapon* NewWeaponData);

protected:
    void CheckCollision();

    bool TryExcludeActor(AActor* HitActor);

    UFUNCTION(meta = (DeprecatedFunction))
    void DEPRECATED_DrawTraceLine(FVector prevVec_, FVector currVec_, bool bDrawTraceLine);

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (ExposeOnSpawn = 1))
    bool bEnableDrawTraceLine;

    UFUNCTION(BlueprintCallable)
    void StartSwing(float ActionMulti_Damage, float ActionMulti_PostureDamage);
    UFUNCTION(BlueprintCallable)
    void EndSwing();

    bool GetIsSwinging() const { return bAttackingSwitch; }
};

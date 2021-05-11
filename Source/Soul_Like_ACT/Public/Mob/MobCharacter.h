// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulCharacterBase.h"
#include "MobCharacter.generated.h"

class UDA_FXCollection;
class UMob_TargetingComponent;

UENUM()
enum class EAI_ActionType : uint8 { Attack, Parry, Dodge, DashBack, Walk };

UCLASS(Abstract)
class SOUL_LIKE_ACT_API AMobCharacter : public ASoulCharacterBase
{
private:
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UMob_TargetingComponent* TargetingComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gears)
    class AWeaponActor* Weapon;

public:
    // Sets default values for this pawn's properties
    AMobCharacter();
    //
    // UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    // class UMobActionManager* ActionManager;

protected:
    virtual void ForceOverrideFacingDirection(float Alpha) override;

    virtual void HandleOnDead(const FHitResult& HitInfo,
       const FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter,
       AActor* DamageCauser) override;

public:
    UMob_TargetingComponent* GetTargetingComponent() const { return TargetingComponent; }

    UFUNCTION(BlueprintCallable, category = "AI Controller")
    class AMobController* GetMobController() const;

   UFUNCTION(BlueprintCallable, Category = "AI Controller")
    class UMobRageManager* GetRageManager() const;

    virtual AWeaponActor* GetWeaponActor() override {return Weapon;}
};

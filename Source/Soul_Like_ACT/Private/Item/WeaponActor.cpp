// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/WeaponActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "BPFL/BPFL_AbilitySystem.h"
#include "SoulCharacterBase.h"
#include "DrawDebugHelpers.h"
#include "Soul_Like_ACT.h"
#include "BPFL/BPFL_Equipment.h"

AWeaponActor::AWeaponActor()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
    
    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComp");
    MeshComp->SetupAttachment(RootComponent);
}

void AWeaponActor::BeginPlay()
{
    Super::BeginPlay();
}

void AWeaponActor::CheckCollision()
{
    HitPoints_LastFrame = HitPOints_CurrFrame;
    for (int i = WeaponSpecs->HandleLength; i < WeaponSpecs->BladeLength; i += 30)
    {
        const int32 size_ = (i - WeaponSpecs->HandleLength) / 30;
        
        HitPOints_CurrFrame[size_] = GetActorLocation() + i * GetActorUpVector();

        DEPRECATED_DrawTraceLine(HitPoints_LastFrame[size_], HitPOints_CurrFrame[size_], bEnableDrawTraceLine);
    }

    const int32 size_ = (WeaponSpecs->BladeLength - WeaponSpecs->HandleLength) / 30;

    HitPOints_CurrFrame[size_] = GetActorLocation() + WeaponSpecs->BladeLength * GetActorUpVector();

    DEPRECATED_DrawTraceLine(HitPoints_LastFrame[size_], HitPOints_CurrFrame[size_], bEnableDrawTraceLine);
}

void AWeaponActor::DEPRECATED_DrawTraceLine(FVector prevVec_, FVector currVec_, bool bDrawTraceLine)
{
    TArray<FHitResult> Hits;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetInstigator());

    const bool bIsHit = GetWorld()->LineTraceMultiByChannel(Hits, prevVec_, currVec_, ECC_Pawn, QueryParams);
    if (bIsHit)
    {
        if (bDrawTraceLine)
            DrawDebugLine(GetWorld(), prevVec_, currVec_, FColor::Green, false, 2.f, 0, 1.f);

        for (const auto& TempHitRes : Hits)
        {
            ASoulCharacterBase* TargetPawn = Cast<ASoulCharacterBase>(TempHitRes.GetActor());
            if (TargetPawn
                && ASoulCharacterBase::IsInRivalFaction(Cast<ASoulCharacterBase>(GetInstigator()), TargetPawn)
                && TryExcludeActor(TargetPawn))
            {
                // Send hit event for Damage
                if(TargetPawn && !TargetPawn->IsPendingKill())
                {

                    /**
                     * Calculate damage and posture multi
                     * Pass these 2 values to JsonWrapper
                     * We can access the JsonObject as 1st optional object in EventData
                     */
                    auto JsonWrapper = USoulJsonObjectWrapper::MakeSoulJsonObject();
                    float SumDamageMulti = ActionDamageMulti * WeaponSpecs->WeaponDamageMulti;
                    float SumPostureDamageMulti = ActionPostureDamageMulti * WeaponSpecs->WeaponPostureDamageMulti;
                    JsonWrapper->SetActionDamageMulties(SumDamageMulti, SumPostureDamageMulti, 1.f);

                    FGameplayEventData EventData;
                    EventData.OptionalObject = JsonWrapper;
                    
                    UBPFL_AbilitySystem::CreateEventData(TargetPawn, GetOwner(), TempHitRes, FGameplayTag(), 1.f, EventData);
                    UBPFL_AbilitySystem::SendGameplayEventToActor(GetOwner(), UBPFL_Equipment::GetWeaponHitTag(), EventData);
                }
            }
        }
    }
    else if (bDrawTraceLine)
        DrawDebugLine(GetWorld(), prevVec_, currVec_, FColor::Red, 0, 2.f, 0, 1.f);
}

/*
* Exclude the actor which takes the impact
* Return true if the Actor has not been impacted
*/
bool AWeaponActor::TryExcludeActor(AActor* HitActor)
{
    if (MyTargets.Contains(HitActor))
    {
        return false;
    }
    
    MyTargets.Add(HitActor);
    return true;
}

void AWeaponActor::StartSwing(float ActionMulti_Damage, float ActionMulti_PostureDamage)
{
    bAttackingSwitch = true;

    //Damage Multi
    ActionDamageMulti = ActionMulti_Damage;
    ActionPostureDamageMulti = ActionMulti_PostureDamage;

    HitPOints_CurrFrame.Reset();
    MyTargets.Reset();

    // Send sweeping sound event
    FGameplayEventData EventData;
    UBPFL_AbilitySystem::CreateEventDataWithoutHitResult(nullptr, GetOwner(), FGameplayTag(), 1.f, EventData);
    UBPFL_AbilitySystem::SendGameplayEventToActor(GetOwner(),
                                                  UBPFL_Equipment::GetWeaponAttackSoundTag(ESoundSelector_MeleeAttack::Attacking),
                                                  EventData);
    
    for (int i = WeaponSpecs->HandleLength; i <= WeaponSpecs->BladeLength; i += 30)
    {
        HitPOints_CurrFrame.Push(GetActorLocation() + i * GetActorUpVector());
    }
}

void AWeaponActor::EndSwing()
{
    bAttackingSwitch = false;
}

void AWeaponActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bAttackingSwitch)
        CheckCollision();
}

void AWeaponActor::SetupWeaponSpecs(UPA_Weapon* NewWeaponData)
{
    WeaponSpecs = NewWeaponData;

    //todo update mesh
    if(!WeaponSpecs->WeaponMesh)
    {
        LOG_FUNC_FAILURE("Weapon mesh is not set");
        return;
    }

    MeshComp->SetSkeletalMesh(WeaponSpecs->WeaponMesh);
    LOG_FUNC_SUCCESS(WeaponSpecs->WeaponMesh->GetName() + " is equipped");        
}

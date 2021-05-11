// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_UseGA_Duration.h"
#include "AIController.h"
#include "AbilitySystemGlobals.h"
#include "Soul_Like_ACT.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Abilities/SoulAbilitySystemComponent.h"
#include "Abilities/SoulGameplayAbility.h"

UBTT_UseGA_Duration::UBTT_UseGA_Duration()
{
    NodeName = "Use Casting Ability";
    bCreateNodeInstance = true;
    bNotifyTick = false;
}

EBTNodeResult::Type UBTT_UseGA_Duration::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    USoulAbilitySystemComponent* ASC = Cast<USoulAbilitySystemComponent>(
        UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(
            Cast<AAIController>(OwnerComp.GetOwner())->GetPawn()));
    if (!ASC)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Failed to get ASC");
        return EBTNodeResult::Failed;
    }

    BTC = &OwnerComp;

    UClass* GA_Class = bUseKey
                           ? OwnerComp.GetBlackboardComponent()->GetValueAsClass(GA_Melee_CDO.SelectedKeyName)
                           : *AbilityClass;

    if(!GA_Class->IsValidLowLevel())
    {
        LOG_FUNC_ERROR("No valid GA_Class is found");
        return EBTNodeResult::Aborted;
    }
    
    if (!ASC->IsAbilityGiven(GA_Class))
    {
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Ability is not given to ASC");
        ASC->GiveAbility(FGameplayAbilitySpec(GA_Class));
    }

    if (GA_Class && GA_Class->IsChildOf(UActiveAbility::StaticClass()))
    {
        FOnGameplayAbilityEnded::FDelegate DelegateObj = FOnGameplayAbilityEnded::FDelegate::CreateUObject(
                                                            this, &UBTT_UseGA_Duration::OnGA_Ended);
        if (!ASC->TryActivateAbilityByClassWithDelegate(GA_Handle, GA_Class, &DelegateObj))
            return EBTNodeResult::Failed;
    }
    
    const auto AbilitySpec = ASC->FindAbilitySpecFromHandle(GA_Handle);

    if(AbilitySpec->Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::InstancedPerActor
        && AbilitySpec->GetPrimaryInstance()->IsA(UActiveAbility::StaticClass()))
    {

        Cast<UActiveAbility>(AbilitySpec->GetPrimaryInstance())->DEPRECATED_EndLatentAbility(true, EndSection, EndLatency);
    }
    return EBTNodeResult::InProgress;
}

void UBTT_UseGA_Duration::OnGA_Ended(UGameplayAbility* Ability)
{
    GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "EndLatentAbility");

    FinishLatentTask(*BTC, EBTNodeResult::Succeeded);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_UseGA.h"
#include "AIController.h"
#include "AbilitySystemGlobals.h"
#include "Soul_Like_ACT.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Abilities/SoulAbilitySystemComponent.h"
#include "Abilities/SoulGameplayAbility.h"


UBTT_UseGA::UBTT_UseGA(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    NodeName = "Use Melee Ability";
    bCreateNodeInstance = true;
    bNotifyTick = false;
}

EBTNodeResult::Type UBTT_UseGA::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
    
    if(!ASC->IsAbilityGiven(GA_Class))
    {
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Ability is not given to ASC");

        ASC->GiveAbility(FGameplayAbilitySpec(GA_Class));
    }
        
    if(GA_Class && GA_Class->IsChildOf(UGA_Melee::StaticClass()))
    {
        FGameplayAbilitySpecHandle _handle;

        FOnGameplayAbilityEnded::FDelegate DelegateObj = FOnGameplayAbilityEnded::FDelegate::CreateUObject(
                                                                                            this, &UBTT_UseGA::OnGA_Ended);

        if (!ASC->TryActivateAbilityByClassWithDelegate(_handle, GA_Class, &DelegateObj))
            return EBTNodeResult::Failed;
    }
        
    return EBTNodeResult::InProgress;
}



void UBTT_UseGA::OnGA_Ended(class UGameplayAbility* Ability)
{
    FinishLatentTask(*BTC, EBTNodeResult::Succeeded);
}

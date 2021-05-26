#include "BPFL/BPFL_Utilities.h"
#include "Soul_Like_ACT.h"
#include "Abilities/SoulAttributeSet.h"
#include "Abilities/SoulModifierManager.h"
#include "Player/EquipmentManager.h"
#include "Player/InventoryComponent.h"
//
// void UBPFL_Utilities::GetRenderedActors(TArray<AActor*>& CurrentlyRenderedActors, float MinRecentTime/*= 0.01f*/)
// {
//     //Empty any previous entries
//     CurrentlyRenderedActors.Empty();
//      
//     //Iterate Over Actors
//     for ( TObjectIterator<AActor> Itr; Itr; ++Itr )
//     {
//         if (Itr->GetLastRenderTime() > MinRecentTime)
//         {
//             CurrentlyRenderedActors.Add( * Itr);
//         }
//     }
// }
//
// void UBPFL_Utilities::GetNotRenderedActors(TArray<AActor*>& CurrentlyNotRenderedActors, float MinRecentTime/*= 0.01f*/)
// {
//     //Empty any previous entries
//     CurrentlyNotRenderedActors.Empty();
//      
//     //Iterate Over Actors
//     for ( TObjectIterator<AActor> Itr; Itr; ++Itr )
//     {
//         if (Itr->GetLastRenderTime() <= MinRecentTime)
//         {
//             CurrentlyNotRenderedActors.Add( * Itr);
//         }
//     }
// }

UEquipmentManager* UBPFL_Utilities::GetEquipmentManager(UObject* WorldContext)
{
    const auto PC = WorldContext->GetWorld()->GetFirstPlayerController();
    if(PC)
    {
        const auto Comp = PC->FindComponentByClass(UEquipmentManager::StaticClass());
        if(Comp)
            return Cast<UEquipmentManager>(Comp);
    }
    return nullptr;
}

UInventoryComponent* UBPFL_Utilities::GetInventoryManager(UObject* WorldContext)
{
    const auto PC = WorldContext->GetWorld()->GetFirstPlayerController();
    if(PC)
    {
        const auto Comp = PC->FindComponentByClass(UInventoryComponent::StaticClass());
        if(Comp)
            return Cast<UInventoryComponent>(Comp);
    }
    return nullptr;
}

USoulModifierManager* UBPFL_Utilities::GetModifierManager(UObject* WorldContext)
{
    const auto PC = WorldContext->GetWorld()->GetFirstPlayerController();
    if(PC)
    {
        const auto Pawn = PC->GetPawn();
        if(Pawn)
        {
            const auto ModManager = Pawn->FindComponentByClass(USoulModifierManager::StaticClass());
            if(ModManager)
            {
                return Cast<USoulModifierManager>(ModManager);
            }
            else
            {
                LOG_FUNC_FAILURE(ModManager->GetName());
            }
        }
    }
    return nullptr;
}

bool UBPFL_Utilities::MergeModifierArray(TArray<FSoulModifier>& A)
{
    if(A.Num() == 0)
    {
        LOG_FUNC_ERROR("Invalid Input array")
        return false;
    }
    else if (A.Num() == 1)
    {
        LOG_FUNC_FAILURE("Input array size 1")
        return true;
    }
    else
    {
        int Head = 0, Index = 1;
        int Tail = A.Num() - 1;

        while(Head < Tail)
        {
            while(Index <= Tail)
            {
                if(A[Index].Modifier_CDO == A[Head].Modifier_CDO)
                {
                    A[Head].CombineModifier(A[Index]);
                    A.RemoveAtSwap(Index);
                    --Tail;
                }
                else
                {
                    ++Index;
                }
            }
            ++Head;
            Index = Head + 1;
        }

        return true;
    }
}

FString UBPFL_Utilities::GetFormattedAttributeValue(FGameplayAttribute Attribute, float Value)
{
    if(Attribute == USoulAttributeSet::GetHealthAttribute())
    {
        return FString::SanitizeFloat(Value);
    }
    //Default
    else
    {
        return FString::SanitizeFloat(Value);
    }
}

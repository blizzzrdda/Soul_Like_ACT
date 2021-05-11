#include "BPFL/BPFL_ItemMaker.h"
#include "Soul_Like_ACT.h"
#include "BPFL/BPFL_Utilities.h"

bool UBPFL_ItemMaker::SwapItemBetweenComponents(TScriptInterface<IInterface_InventoryManagement> InventA, uint8 SlotA,
    TScriptInterface<IInterface_InventoryManagement> InventB, uint8 SlotB)
{
    if(!InventA || !InventB)
    {
        LOG_FUNC_ERROR("One of the inventory objects is invalid");
        return false;
    }

    if(!InventA->IsSlotValid(SlotA))
    {
        LOG_FUNC_ERROR("Slot A is invalid");
        return false;
    }
    
    if(!InventB->IsSlotValid(SlotB))
    {
        LOG_FUNC_ERROR("Slot B is invalid");
        return false;
    }

    InventA->SwapItemWithOtherInterface(SlotA, InventB, SlotB);
    return true;
}

bool UBPFL_ItemMaker::SumModifiersOfItem(const FInventoryItem& InventoryItem, TArray<FSoulModifier>& OutMods,
    int32& EmptySlots)
{
    TArray<FSoulModifier> GarbageBag1, GarbageBag2; 
    GetModifiersOfItem(InventoryItem, OutMods, GarbageBag1,GarbageBag2,EmptySlots);

    return UBPFL_Utilities::MergeModifierArray(OutMods);
}

bool UBPFL_ItemMaker::GetModifiersOfItem(const FInventoryItem& InventoryItem, TArray<FSoulModifier>& TotalMods,
                                         TArray<FSoulModifier>& ItemMods, TArray<FSoulModifier>& JewelMods, int32& EmptySlots)
{
    if(!InventoryItem.IsValid()) return false;

    //Get mods from item
    for(auto& ModSlot : InventoryItem.ItemBase->ItemModifiers)
    {
        if(!ModSlot.IsValid())
            LOG_FUNC_ERROR("Mod slot is invalid")
        else
        {
            TotalMods.Add(ModSlot);
            ItemMods.Add(ModSlot);
        }
    }

    //Get mods from jewels
    for(auto& Jewel : InventoryItem.Jewels)
    {
        if(Jewel)
        {
            for(auto& ModSlot : Jewel->ItemModifiers)
            {
                if(!ModSlot.IsValid())
                    LOG_FUNC_ERROR("Mod slot is invalid")
                else
                {
                    TotalMods.Add(ModSlot);
                    JewelMods.Add(ModSlot);
                }
            }
        }
        else
        {
            ++EmptySlots;
        }
    }

    if(EmptySlots > InventoryItem.ItemBase->MaxJewelSockets)
    {
        LOG_FUNC_ERROR("Check inventory item: " + InventoryItem.ItemBase->GetName() + " .It has more empty sockets:" + FString::FromInt(EmptySlots) +
            " than max sockets:" + FString::FromInt(InventoryItem.ItemBase->MaxJewelSockets));
        return false;
    }
    return true;
}

void UBPFL_ItemMaker::GetItemAttributes(const FInventoryItem& InventoryItem, TArray<FText>& Attributes,
                                        TArray<FText>& Values)
{
    if(!InventoryItem.IsValid()) return;

    const auto attribute_n_values = InventoryItem.ItemBase->GetItemAttributes();

    for (auto _pair : attribute_n_values)
    {
        Attributes.Add(FText::FromString(_pair.Key));
        Values.Add(FText::FromString(FString::SanitizeFloat(_pair.Value)));
    }
}

FInventoryItem UBPFL_ItemMaker::MakeItemWithAssetID(FPrimaryAssetId PA_ID, TArray<FPrimaryAssetId> Jewels)
{
    auto AssetManager = USoulAssetManager::Get();

    if(AssetManager && PA_ID.IsValid())
    {
        FInventoryItem InventoryItem(AssetManager->ForceLoadItem(PA_ID));

        const uint8 JewelsNum = FMath::Min(Jewels.Num(), InventoryItem.ItemBase->MaxJewelSockets);
        
        for(uint8 i = 0; i < JewelsNum; ++i)
        {
            const auto JewelId = Jewels[i];
            const auto PA_JewelPtr = AssetManager->ForceLoadItem(JewelId);
            const auto PA_Jewel = Cast<UPA_Jewel>(PA_JewelPtr);
            if(!PA_Jewel) continue;

            InventoryItem.Jewels[i] = PA_Jewel;
        }
        
        return InventoryItem;
    }

    return FInventoryItem();
}

EJewelSocketStatus UBPFL_ItemMaker::CheckItemSocketStatus(FInventoryItem& Item, int32 SocketIndex)
{
    if(Item.IsValid())
    {
        if(Item.IsJewelSocketValid(SocketIndex))
        {
            if(Item.Jewels[SocketIndex] == nullptr)
                return EJewelSocketStatus::Empty;
            else if (IsValid(Item.Jewels[SocketIndex]))
                return EJewelSocketStatus::Inserted;
        }
    }
    return EJewelSocketStatus::InvalidJewelSocket;
}

bool UBPFL_ItemMaker::SetJewelSocket(const TScriptInterface<IInterface_InventoryManagement>& Src, uint8 InventSlot,
                                     uint8 JewelSlot, UPA_Jewel* InJewel, UPA_Jewel*& OutJewel)
{
    if(!Src)
    {
        LOG_FUNC_ERROR("Inventory interface is invalid")
        return false;
    }

    if(!Src->IsSlotValid(InventSlot))
    {
        LOG_FUNC_ERROR("Invent slot is out of range")
        return false;
    }

    bool IsInventItemValid;
    auto& LocalItem = Src->GetInventoryItemRef(InventSlot, IsInventItemValid);
    
    if(!IsInventItemValid)
    {
        LOG_FUNC_FAILURE("The invent item is invalid")
        return false;
    }

    if(!LocalItem.IsJewelSocketValid(JewelSlot))
    {
        LOG_FUNC_FAILURE("The jewel slot is invalid on the item")
        return false;
    }
    
    OutJewel = LocalItem.Jewels[JewelSlot];
    LocalItem.Jewels[JewelSlot] = InJewel;
    return true;
}

int32 UBPFL_ItemMaker::GetFirstAvailableJewelSlot(const FInventoryItem& Item)
{
    if(!Item.IsValid()) return -1;

    for(int32 i = 0; i < Item.ItemBase->MaxJewelSockets; ++i)
    {
        if(Item.Jewels[i] == nullptr)
            return i;
    }

    return -1;
}

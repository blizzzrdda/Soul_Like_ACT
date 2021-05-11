#include "Player/InventoryComponent.h"

#include "Soul_Like_ACT.h"
#include "BPFL/BPFL_ItemMaker.h"

const int32 UInventoryComponent::InventorySize = 40;

inline UInventoryComponent::UInventoryComponent()
{
    InventoryItems.Init(FInventoryItem(), InventorySize);
}

bool UInventoryComponent::SetJewelSocket(int32 ItemIndex, UPA_Jewel* InJewel, int32 SocketIndex,
                                         UPA_Jewel*& OutJewel, bool bFirstEmptySlot)
{
    if(!IsSlotValid(ItemIndex))
    {
        LOG_FUNC_FAILURE("Invalid EquipmentSlot");
        return false;  
    }
    auto& InventItem = InventoryItems[ItemIndex];

    const int32 ValidSocketIndex = bFirstEmptySlot
        ? UBPFL_ItemMaker::GetFirstAvailableJewelSlot(InventItem)
        : SocketIndex;

    const bool Result = UBPFL_ItemMaker::SetJewelSocket(this, ItemIndex, ValidSocketIndex, InJewel, OutJewel);

    if(Result)
        Notify_SlottedItemChanged(ItemIndex);

    return Result;
}

bool UInventoryComponent::SetItemToSlot(const FInventoryItem& InventoryItem, uint8 Slot, FInventoryItem& OutItem,
                                        bool bAutoFindAvailableSlot)
{
    if(bAutoFindAvailableSlot)
    {
        if(!InventoryItem.IsValid())
        {
            LOG_FUNC_ERROR("Item is empty while bAutoFindAvailableSlot is on");
            return false;
        }
        
        uint8 FirstFreeSlot;
        if(!GetFirstEmptySlot(FirstFreeSlot))
        {
            LOG_FUNC_ERROR("Item is empty while bAutoFindAvailableSlot is on");
            return false;
        }

        AddItem_Unsafe(InventoryItem, FirstFreeSlot, OutItem);
    }
    else
    {
        if(!IsSlotValid(Slot))
        {
            LOG_FUNC_ERROR("Slot out of range");
            return false;
        }

        if(!InventoryItem.IsValid())
            RemoveItem_Unsafe(Slot, OutItem);
        else
            AddItem_Unsafe(InventoryItem, Slot, OutItem);
    }

    return true;
}

bool UInventoryComponent::SwapItemWithOtherInterface(uint8 FromSlot, TScriptInterface<IInterface_InventoryManagement> ToComponent,
                                                     uint8 ToSlot)
{
    if(!ToComponent)
    {
        LOG_FUNC_ERROR("Cannot find ToComponent");
        return false;
    }
    
    if(!IsSlotValid(FromSlot))
    {
        LOG_FUNC_ERROR("From Slot is out of range");
        return false; 
    }
    if(!ToComponent->IsSlotValid(ToSlot))
    {
        LOG_FUNC_ERROR("From Slot is out of range");
        return false; 
    }

    FInventoryItem ItemToReceive, Garbage; 
    ToComponent->SetItemToSlot(InventoryItems[FromSlot], ToSlot, ItemToReceive, false);
    SetItemToSlot(ItemToReceive, FromSlot, Garbage, false);

    return true;
}

void UInventoryComponent::AddItem_Unsafe(const FInventoryItem& InventoryItem, int32 ToSlot, FInventoryItem& OutItem)
{
    OutItem = InventoryItems[ToSlot];
    InventoryItems[ToSlot] = InventoryItem;    
    
    Notify_SlottedItemChanged(ToSlot);
}

void UInventoryComponent::RemoveItem_Unsafe(uint8 Slot, FInventoryItem& OutInventoryItem)
{
    OutInventoryItem = InventoryItems[Slot];
    InventoryItems[Slot] = FInventoryItem();
    
    Notify_SlottedItemChanged(Slot);
}

FInventoryItem& UInventoryComponent::GetInventoryItemRef(uint8 Slot, bool& IsValid)
{
    if(IsSlotValid(Slot))
    {
        IsValid = InventoryItems[Slot].IsValid();
        return InventoryItems[Slot];
    }
    else
    {
        static auto EmptyInventItem = FInventoryItem();
        IsValid = false;
        return EmptyInventItem;
    }
}

bool UInventoryComponent::GetFirstEmptySlot(uint8& Slot) const
{
    for(int32 i = 0; i < InventorySize; ++i)
    {
        if(!InventoryItems[i].IsValid())
        {
            Slot = i;
            return true;
        }
    }
    return false;
}

void UInventoryComponent::GetEquipSlotAtIndex(uint8 Slot, EEquipmentSlot& EquipSlot) const
{
    if(IsSlotValid(Slot))
        EquipSlot = InventoryItems[Slot].ItemBase->EquipSlot;
    else
        EquipSlot = EEquipmentSlot::InvalidEquipmentSlot;
}

void UInventoryComponent::Notify_SlottedItemChanged(int32 ItemSlot) const
{
    if(OnSlottedItemChanged.IsBound())
        OnSlottedItemChanged.Broadcast(ItemSlot);
}

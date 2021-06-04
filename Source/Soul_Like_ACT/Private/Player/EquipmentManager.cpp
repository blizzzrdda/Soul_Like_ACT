// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/EquipmentManager.h"
#include "Soul_Like_ACT.h"
#include "Abilities/SoulModifierManager.h"
#include "BPFL/BPFL_Equipment.h"
#include "BPFL/BPFL_ItemMaker.h"
#include "BPFL/BPFL_Utilities.h"
#include "Item/WeaponActor.h"
#include "Player/SoulPlayerController.h"

UEquipmentManager::UEquipmentManager()
{
    Equipments.Init(FInventoryItem(), MAX_EQUIP_SLOTS);
}

bool UEquipmentManager::SetJewelSocket(EEquipmentSlot EquipSlot, UPA_Jewel* InJewel, int32 SocketIndex, UPA_Jewel*& OutJewel, bool bFirstEmptySlot)
{
    if(EquipSlot == EEquipmentSlot::InvalidEquipmentSlot)
    {
        LOG_FUNC_ERROR("Invalid EquipmentSlot");
        return false;  
    }

    //WARNING: UBPFL_ItemMaker::GetFirstAvailableJewelSlot returns -1 if item is invalid
    const int32 ValidSocketIndex = bFirstEmptySlot
        ? UBPFL_ItemMaker::GetFirstAvailableJewelSlot(Equipments[static_cast<int32>(EquipSlot)])
        : SocketIndex;

    const bool Result = UBPFL_ItemMaker::SetJewelSocket(this, static_cast<uint8>(EquipSlot), ValidSocketIndex, InJewel, OutJewel);

    if(Result)
        Notify_OnEquipmentChanged(EquipSlot);

    return Result;
}

bool UEquipmentManager::SetItemToSlot(const FInventoryItem& InventoryItem, uint8 Slot, FInventoryItem& OutItem,
                                      bool bAutoFindAvailableSlot)
{
    if(bAutoFindAvailableSlot)
    {
        //Remove current item if there is no item to add in
        if(!InventoryItem.IsValid())
        {
            LOG_FUNC_ERROR("Item is empty while bAutoFindAvailableSlot is on");
            return false;
        }
        else
        {
            //check the the euqipment slot
            if(InventoryItem.ItemBase->EquipSlot == EEquipmentSlot::InvalidEquipmentSlot)
            {
                LOG_FUNC_ERROR("Item is not a weapon or armour type");
                return false;
            }
            
            AddItem_Unsafe(InventoryItem, OutItem);
        }
    }
    else
    {
        //Make sure slot range is valid, e.g. >=0 && <8
        if(!IsSlotValid(Slot))
        {
            LOG_FUNC_ERROR("Slot is out of range");
            return false;
        }
        
        //Remove current item if there is no item to add in
        if(!InventoryItem.IsValid())
        {
            RemoveItem_Unsafe(static_cast<EEquipmentSlot>(Slot), OutItem);
        }
        else
        {
            //Ensure the the euqipment slots match
            if(InventoryItem.ItemBase->EquipSlot != static_cast<EEquipmentSlot>(Slot))
            {
                LOG_FUNC_ERROR("Item is not a weapon or armour type");
                return false;
            }
            
            AddItem_Unsafe(InventoryItem, OutItem);
        }
    }

    return true;
}

bool UEquipmentManager::SwapItemWithOtherInterface(uint8 FromSlot, TScriptInterface<IInterface_InventoryManagement> ToComponent,
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
    ToComponent->SetItemToSlot(Equipments[FromSlot], ToSlot, ItemToReceive, false);
    SetItemToSlot(ItemToReceive, FromSlot, Garbage, false);

    return true;
}

FInventoryItem& UEquipmentManager::GetInventoryItemRef(uint8 Slot, bool& IsValid)
{
    if(IsSlotValid(Slot))
    {
        IsValid = Equipments[Slot].IsValid();
        return Equipments[Slot];
    }
    else
    {
        static auto EmptyInventItem = FInventoryItem();
        IsValid = false;
        return EmptyInventItem;
    }
}

void UEquipmentManager::UpdateWeaponActor(UPA_Weapon* WeaponData)
{
    if(CurrWeaponActor)
        CurrWeaponActor->Destroy();

    if(WeaponData)
    {
        AController* OwnerControl = Cast<AController>(GetOwner());
        if(!OwnerControl || !OwnerControl->GetPawn())
        {
            LOG_FUNC_ERROR("Cannot find owner controller");
            return;
        }

        APawn* PlayerPawn = OwnerControl->GetPawn();
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = PlayerPawn;
        SpawnParams.Instigator = Cast<APawn>(PlayerPawn);

        //spawn world actor
        CurrWeaponActor = GetWorld()->SpawnActor<AWeaponActor>(AWeaponActor::StaticClass(), FTransform::Identity, SpawnParams);
        CurrWeaponActor->SetupWeaponSpecs(WeaponData);

        //Attach to SK
        FName SocketName;
        if(UBPFL_Equipment::StanceToSocketName(WeaponData->DefaultStance, SocketName))
        {
            CurrWeaponActor->AttachToComponent(PlayerPawn->FindComponentByClass<USkeletalMeshComponent>(),
                                           FAttachmentTransformRules::SnapToTargetNotIncludingScale,
                                           SocketName);
        }
    }
}

void UEquipmentManager::AddItem_Unsafe(const FInventoryItem& InItem, FInventoryItem& OutItem)
{
    const auto Index = static_cast<uint8>(InItem.ItemBase->EquipSlot);
    OutItem = Equipments[Index];
    Equipments[Index] = InItem;

    //Update world actor
    if(InItem.IsValid() && InItem.ItemBase->ItemType == USoulAssetManager::WeaponItemType)
    {
        UpdateWeaponActor(Cast<UPA_Weapon>(InItem.ItemBase));
    }
    
    Notify_OnEquipmentChanged(InItem.ItemBase->EquipSlot);
}

void UEquipmentManager::RemoveItem_Unsafe(EEquipmentSlot Slot, FInventoryItem& OutItem)
{
    OutItem = Equipments[static_cast<uint8>(Slot)];
    Equipments[static_cast<uint8>(Slot)] = FInventoryItem();

    //remove world actor
    UpdateWeaponActor(nullptr);
    
    Notify_OnEquipmentChanged(Slot);
}

void UEquipmentManager::Notify_OnEquipmentChanged(EEquipmentSlot EquipSlot) const
{
    if(OnEquipmentChanged.IsBound())
        OnEquipmentChanged.Broadcast(static_cast<int32>(EquipSlot));
        
    //Update modifiers and abilities
    auto MM = UBPFL_Utilities::GetModifierManager(GetWorld());
    if(MM)  MM->UpdateAbilitiesAndModifiers();
    else    LOG_FUNC_FAILURE("Cannot find SoulModifierManager")
}
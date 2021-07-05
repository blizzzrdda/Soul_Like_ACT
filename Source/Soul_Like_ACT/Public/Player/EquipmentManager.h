// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/InventSlot.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Interface_InventoryManagement.h"
#include "EquipmentManager.generated.h"

class AWeaponActor;

static uint8 MAX_EQUIP_SLOTS = 8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API UEquipmentManager : public UActorComponent, public IInterface_InventoryManagement
{
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentChanged, int32, Slot);
    
    GENERATED_BODY()


public:
    UEquipmentManager();

    UPROPERTY(VisibleAnywhere, Category = Euqipment)
    AWeaponActor* CurrWeaponActor;
    
    UPROPERTY(BlueprintReadOnly, Category = Inventory)
    TArray<FInventoryItem> Equipments;
    
    UFUNCTION(BlueprintCallable, Category = Inventory)
    bool SetJewelSocket(EEquipmentSlot EquipSlot, UPA_Jewel* InJewel, int32 SocketIndex, UPA_Jewel*& OutJewel, bool bFirstEmptySlot=true);


    virtual bool SetItemToSlot(const FInventoryItem& InventoryItem, uint8 Slot, FInventoryItem& OutItem,
                               bool bAutoFindAvailableSlot=true) override;


    virtual bool SwapItemWithOtherInterface(uint8 FromSlot,
											TScriptInterface<IInterface_InventoryManagement> ToComponent, uint8 ToSlot) override;

    virtual bool IsSlotValid(uint8 Slot) const override
    {
        return Slot < MAX_EQUIP_SLOTS;
    }

    virtual FInventoryItem& GetInventoryItemRef(uint8 Slot, bool& IsValid) override;

    UPROPERTY(BlueprintAssignable, Category = Inventory)
    FOnEquipmentChanged OnEquipmentChanged;
    
protected:
    void UpdateWeaponActor(UPA_Weapon* WeaponData);
        
    /**
     *  Add the item to the slot automatically.
     *  WARNING: It doesn't validate the item.
     */
    void AddItem_Unsafe(const FInventoryItem& InItem, FInventoryItem& OutItem);
    void RemoveItem_Unsafe(EEquipmentSlot Slot, FInventoryItem& OutItem);
    
    UFUNCTION(BlueprintCallable, Category = Inventory)
    void Notify_OnEquipmentChanged(EEquipmentSlot EquipSlot) const;
};
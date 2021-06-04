// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Interface_InventoryManagement.h"
#include "Types/InventSlot.h"
#include "InventoryComponent.generated.h"

class AWeaponActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API UInventoryComponent : public UActorComponent, public IInterface_InventoryManagement
{
private:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlottedItemChanged, int32, ItemSlot);

    GENERATED_BODY()

public:
    UInventoryComponent();

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Inventory)
    TArray<FInventoryItem> InventoryItems;

    /**
     *  Add FInventoryItem to the inventory component
     *  @bAutoFindAvailableSlot It will disable Slot parameter if it's true.
     */
    UFUNCTION(BlueprintCallable, Category = Inventory)
    bool SetJewelSocket(int32 ItemIndex, UPA_Jewel* InJewel, int32 SocketIndex, UPA_Jewel*& OutJewel, bool bFirstEmptySlot=true);

    virtual bool SetItemToSlot(const FInventoryItem& InventoryItem, uint8 Slot, FInventoryItem& OutItem,
                               bool bAutoFindAvailableSlot) override;

    virtual bool SwapItemWithOtherInterface(uint8 FromSlot,
											TScriptInterface<IInterface_InventoryManagement> ToComponent, uint8 ToSlot) override;

    virtual bool IsSlotValid(uint8 Slot) const override
    {
    	return Slot >= 0 && Slot < InventorySize;
    }

	virtual FInventoryItem& GetInventoryItemRef(uint8 Slot, bool& IsValid) override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool GetFirstEmptySlot(uint8& Slot) const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void GetEquipSlotAtIndex(uint8 Slot, EEquipmentSlot& EquipSlot) const;
	
    UPROPERTY(BlueprintAssignable, Category = Inventory)
    FOnSlottedItemChanged OnSlottedItemChanged;

protected:
	const static int32 InventorySize;

	void AddItem_Unsafe(const FInventoryItem& InventoryItem, int32 ToSlot, FInventoryItem& OutItem);
	void RemoveItem_Unsafe(uint8 Slot, FInventoryItem& OutInventoryItem);

    UFUNCTION(BlueprintCallable, Category = Inventory)
    void Notify_SlottedItemChanged(int32 ItemSlot) const;
};
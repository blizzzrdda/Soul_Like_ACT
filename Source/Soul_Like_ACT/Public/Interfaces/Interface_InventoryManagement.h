// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/InventSlot.h"
#include "UObject/Interface.h"
#include "Interface_InventoryManagement.generated.h"


UINTERFACE(BlueprintType, MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UInterface_InventoryManagement : public UInterface
{
	GENERATED_BODY()
};

class SOUL_LIKE_ACT_API IInterface_InventoryManagement
{
	GENERATED_BODY()

public:
	/**
	*  It either set the slot with new item
	*  Or free the slot.
	*/
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="InventoryItem"), Category = Inventory)
	virtual bool SetItemToSlot(const FInventoryItem& InventoryItem, uint8 Slot, FInventoryItem& OutItem,
							   bool bAutoFindAvailableSlot = true) = 0;

	/**
	*	Use this to swap item with other components
	*/
	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual bool SwapItemWithOtherInterface(uint8 FromSlot, TScriptInterface<IInterface_InventoryManagement> ToComponent,
											uint8 ToSlot) = 0;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual bool IsSlotValid(uint8 Slot) const = 0;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual UPARAM(ref) FInventoryItem& GetInventoryItemRef(uint8 Slot, bool& IsValid) = 0;
};


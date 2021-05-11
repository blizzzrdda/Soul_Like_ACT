#pragma once
#include "Interfaces/Interface_InventoryManagement.h"
#include "Types/InventSlot.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPFL_ItemMaker.generated.h"


UCLASS()
class UBPFL_ItemMaker : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
    
public:
    /**
     *  Swap items on the slot of 2 inventory  component
     */
    UFUNCTION(BlueprintCallable, Category = Inventory)
    static bool SwapItemBetweenComponents(TScriptInterface<IInterface_InventoryManagement> InventA, uint8 SlotA,
        TScriptInterface<IInterface_InventoryManagement> InventB, uint8 SlotB);

	/**
	 *	The function is useful for calculating the tooltip and updating modifiers
	 *	It does not display each modifier as an Jewel Socket or a single affix on items
	 */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = ItemInfo)
    static bool SumModifiersOfItem(const FInventoryItem& InventoryItem, TArray<FSoulModifier>& OutMods, int32& EmptySlots);

	/**
	 *	It's useful for showing each jewel sockets on items WITHOUT merging modifiers by the same modifier class
	 */
	UFUNCTION(BlueprintCallable, Category = ItemInfo)
	static bool GetModifiersOfItem(const FInventoryItem& InventoryItem, TArray<FSoulModifier>& TotalMods, TArray<FSoulModifier>& ItemMods,
								   TArray<FSoulModifier>& JewelMods, int32& EmptySlots);

    UFUNCTION(BlueprintCallable, Category = ItemInfo)
    static void GetItemAttributes(const FInventoryItem& InventoryItem, TArray<FText>& Attributes, TArray<FText>& Values);

    UFUNCTION(BlueprintCallable, Category = ItemInfo)
    static bool IsInventoryItemValid(const FInventoryItem& InventoryItem) { return InventoryItem.IsValid(); }
    
    //Make item with ID
    UFUNCTION(BlueprintCallable, BlueprintPure, meta = (AutoCreateRefTerm = "Jewels"))
    static FInventoryItem MakeItemWithAssetID(FPrimaryAssetId PA_ID, TArray<FPrimaryAssetId> Jewels);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    static EJewelSocketStatus CheckItemSocketStatus(UPARAM(ref) FInventoryItem& Item, int32 SocketIndex);

	UFUNCTION(BlueprintCallable, Category = Jewel)
	static bool SetJewelSocket(const TScriptInterface<IInterface_InventoryManagement>& Src,
							   uint8 InventSlot, uint8 JewelSlot, UPA_Jewel* InJewel, UPA_Jewel*& OutJewel);

    UFUNCTION(BlueprintCallable, Category = Jewel)
    static int32 GetFirstAvailableJewelSlot(const FInventoryItem& Item);
};
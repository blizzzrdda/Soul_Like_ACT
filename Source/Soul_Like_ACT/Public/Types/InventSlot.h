#pragma once
#include "Item/PA_Item.h"
#include "InventSlot.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem
{
    GENERATED_BODY()

    FInventoryItem()
        :ItemBase(nullptr)
    {}
    
    explicit FInventoryItem(UPA_Item_Abstract* Item)
        : ItemBase(Item)
    {
        Jewels.Init(nullptr, Item->MaxJewelSockets);
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
    UPA_Item_Abstract* ItemBase;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
    TArray<UPA_Jewel*> Jewels;

    FORCEINLINE bool operator==(const FInventoryItem& Other)
    {
        if(Jewels.Num() != Other.Jewels.Num()) return false;

        for(int32 i = 0; i < Jewels.Num(); ++i)
        {
            if(Jewels[i] != Other.Jewels[i]) return false;
        }
        
        return ItemBase == Other.ItemBase;
    }

    inline bool IsValid() const
    {
        const bool isItemValid = ItemBase != nullptr;

        bool isJewelValid = true;
        for(auto i = 0; i < Jewels.Num(); ++i)
        {
            //TODO check this later
            isJewelValid &= Jewels[i] == nullptr ? true : true;
        }

        return isItemValid && isJewelValid;
    }

    inline bool IsJewelSocketValid(uint8 SocketIndex) const
    {
        return SocketIndex < ItemBase->MaxJewelSockets;
    }
};


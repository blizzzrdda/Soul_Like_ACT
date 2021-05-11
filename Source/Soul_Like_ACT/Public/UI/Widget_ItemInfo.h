// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SoulNavWidget.h"
#include "Interfaces/Interface_InventoryManagement.h"
#include "Widget_ItemInfo.generated.h"

struct FItemInfo;
class UTextBlock;
class UImage;
class UVerticalBox;

UENUM(BlueprintType)
enum class EModifierSlotType : uint8
{
    ItemNative,
    JewelSlot
};

UCLASS(Abstract)
class SOUL_LIKE_ACT_API UWidget_ItemInfo : public USoulNavWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn=true))
    TScriptInterface<IInterface_InventoryManagement> CachedInventoryInterface;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn=true))
    uint8 CachedInventSlot;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn=true))
    bool bEditable = true;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* ItemType;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* ItemName;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* ItemDescription;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
    UImage* ItemIcon;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool IsInterfaceValid() const
    {
        if(CachedInventoryInterface)
            return true;
        return false;
    }
};

UCLASS(Abstract)
class SOUL_LIKE_ACT_API UWidget_ItemSlotInfo : public USoulNavWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn=true))
    bool bEditable = true;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn=true))
    TScriptInterface<IInterface_InventoryManagement> CachedInventoryInterface;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn=true))
    uint8 CachedInventSlot;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn=true))
    EModifierSlotType ModifierSlotType;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn=true))
    uint8 CachedModifierSlot;    

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* ItemType;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* ItemName;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* ItemDescription;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool IsInterfaceValid() const
    {
        if(CachedInventoryInterface)
            return true;
        return false;
    }
};
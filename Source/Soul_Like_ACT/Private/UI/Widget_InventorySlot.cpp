// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widget_InventorySlot.h"
#include "Components/Image.h"


void UWidget_InventorySlot::NativePreConstruct()
{
    Super::NativePreConstruct();

    if(DefaultButtonBackground)
    ItemIcon->SetBrushFromTexture(DefaultButtonBackground, false);
}

void UWidget_InventorySlot::SetupSlot(const FInventoryItem& InItemData)
{
    if (InItemData.IsValid())
    {
        ItemIcon->SetBrushFromTexture(InItemData.ItemBase->ItemIcon, false);
    }
    else
    {
        ItemIcon->SetBrushFromTexture(DefaultButtonBackground, false);
    }
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UINavComponent.h"
#include "Interfaces/Interface_HighlightableInventory.h"
#include "Types/InventSlot.h"
#include "Widget_InventorySlot.generated.h"

UCLASS()
class SOUL_LIKE_ACT_API UWidget_InventorySlot : public UUINavComponent, public IInterface_HighlightableSlot
{
    GENERATED_BODY()

public:
    virtual void NativePreConstruct() override;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
    int32 InventorySlot;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Default)
    EEquipmentSlot EquipmentSlot = EEquipmentSlot::InvalidEquipmentSlot;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Default)
    UTexture2D* DefaultButtonBackground;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UImage* ItemIcon;

    UFUNCTION(BlueprintCallable, Category = InventoryUI)
    bool GetIsEquipmentSlot() const { return EquipmentSlot != EEquipmentSlot::InvalidEquipmentSlot; }
    UFUNCTION(BlueprintCallable)
    void SetupSlot(const FInventoryItem& InItemData);
};
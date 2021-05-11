// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UINavComponent.h"
#include "Blueprint/UserWidget.h"
#include "Widget_ModifierSlot.generated.h"

class UTextBlock;

UCLASS()
class SOUL_LIKE_ACT_API UWidget_ModifierSlot : public UUINavComponent
{
    GENERATED_BODY()

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* GA_Name = nullptr;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* GA_Level = nullptr;

public:
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Inventory)
    void Setup(const FText& inp_Name, const FText& inp_Level, bool IsJewel);
};

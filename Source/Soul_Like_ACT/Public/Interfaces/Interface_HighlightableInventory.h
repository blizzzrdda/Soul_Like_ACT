// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Item/PA_Item.h"
#include "UObject/Interface.h"
#include "Interface_HighlightableInventory.generated.h"

UINTERFACE(BlueprintType, MinimalAPI)
class UInterface_HighlightableInventory : public UInterface
{
	GENERATED_BODY()
};

class SOUL_LIKE_ACT_API IInterface_HighlightableInventory
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CancelHighlights();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HighlightWithAssetType(FPrimaryAssetType AssetType, bool bReset=true);

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HighlightWithMultiTypes(const TArray<FPrimaryAssetType>& Types, bool bReset=true);
};

UINTERFACE(BlueprintType, MinimalAPI)
class UInterface_HighlightableSlot : public UInterface
{
	GENERATED_BODY()
};

class SOUL_LIKE_ACT_API IInterface_HighlightableSlot
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	bool IsHighlighting();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHighlight(bool bOn=true);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SwitchWidget();
};

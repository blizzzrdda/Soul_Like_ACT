// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UINavCollection.h"
#include "Interfaces/Interface_HighlightableInventory.h"
#include "SoulNavCollection.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API USoulNavCollection : public UUINavCollection
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNavCollectionSelected, int32, ParentToIndex, int32, ComponentToIndex);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnNavCollectionNav, int32, ParentFromIndex, int32, ParentToIndex,
        int32, ComponentFromIndex, int32, ComponentToIndex);
	
	UPROPERTY(BlueprintAssignable, Category = NavCollection)
	FOnNavCollectionNav Delegate_NavTo;
	UPROPERTY(BlueprintAssignable, Category = NavCollection)
	FOnNavCollectionSelected Delegate_Select;

	virtual void OnNavigate_Implementation(int From, int To, int LocalFrom, int LocalTo) override;
	virtual void OnSelect_Implementation(int Index, int LocalIndex) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = NavCollection)
	void SetCollectionEnabled(bool bEnable);

	UFUNCTION(BlueprintCallable)
	void ForceIncreGrids(int Increment);
};

UCLASS()
class SOUL_LIKE_ACT_API UHighlightableCollection : public USoulNavCollection, public IInterface_HighlightableInventory
{
public:
	virtual void HighlightWithMultiTypes_Implementation(const TArray<FPrimaryAssetType>& Types, bool bReset) override;
private:
	GENERATED_BODY()

public:
};
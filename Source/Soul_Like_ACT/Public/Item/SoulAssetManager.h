// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SoulAssetManager.generated.h"

/**
 * Game implementation of asset manager, overrides functionality and stores game-specific types
 * It is expected that most games will want to override AssetManager as it provides a good place for game-specific loading logic
 * This is used by setting AssetManagerClassName in DefaultEngine.ini
 */
UCLASS()
class SOUL_LIKE_ACT_API USoulAssetManager : public UAssetManager
{
    GENERATED_BODY()

public:
    // Constructor and overrides
    USoulAssetManager()
    {
    }

    virtual void StartInitialLoading() override;

    /** Static types for items */
    static const FPrimaryAssetType JewelItemType;
    static const FPrimaryAssetType WeaponItemType;
    static const FPrimaryAssetType ArmourItemType;

    /** Returns the current AssetManager object */
    UFUNCTION(BlueprintCallable)
    static USoulAssetManager* Get();

    UFUNCTION(BlueprintCallable)
    class UPA_Item_Abstract* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);
};

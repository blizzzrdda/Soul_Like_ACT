// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SoulSaveGame.h"
#include "Item/SoulAssetManager.h"
#include "Player/SoulPlayerCharacter.h"
#include "SoulGameInstanceBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadingFinished);

/**
 * Base class for GameInstance, should be blueprinted
 * Most games will need to make a game-specific subclass of GameInstance
 * Once you make a blueprint subclass of your native subclass you will want to set it to be the default in project settings
 */
UCLASS()
class SOUL_LIKE_ACT_API USoulGameInstanceBase : public UGameInstance
{
    GENERATED_BODY()

public:
    // Constructor
    USoulGameInstanceBase();

    //By Default, It contains all accessible Item Types
    const TArray<FPrimaryAssetType> AllItemTypes{
        USoulAssetManager::ArmourItemType, USoulAssetManager::WeaponItemType,
        USoulAssetManager::JewelItemType
    };

    UFUNCTION(BlueprintCallable, Category = Inventory)
    void GetAllAccessibleItemID(TArray<FPrimaryAssetId>& OutpId);

    UFUNCTION(BlueprintCallable, Category = Inventory)
    void GetItemIDWithType(const FPrimaryAssetType ItemType, TArray<FPrimaryAssetId>& OutpId);

    /** Spawn Floating Damage Widget on screen */
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = UI)
    void SpawnFloatingDamageTextWidget(const AActor* DamageReceiver, const float DamageInput, bool bIsCritical = false);
};
// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SoulAssetManager.h"
#include "Item/PA_Item.h"
#include "AbilitySystemGlobals.h"

const FPrimaryAssetType USoulAssetManager::JewelItemType = TEXT("Jewel");
const FPrimaryAssetType USoulAssetManager::WeaponItemType = TEXT("Weapon");
const FPrimaryAssetType USoulAssetManager::ArmourItemType = TEXT("Armour");

void USoulAssetManager::StartInitialLoading()
{
    Super::StartInitialLoading();

    UAbilitySystemGlobals::Get().InitGlobalData();
}

USoulAssetManager* USoulAssetManager::Get()
{
    USoulAssetManager* This = Cast<USoulAssetManager>(GEngine->AssetManager);

    if (This)
    {
        return This;
    }
    else
    {
        UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be SoulAssetManager!"));
        return NewObject<USoulAssetManager>();
    }
}

UPA_Item_Abstract* USoulAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning /*= true*/)
{
    const FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

    // This does a synchronous load and may hitch
    UPA_Item_Abstract* LoadedItem = Cast<UPA_Item_Abstract>(ItemPath.TryLoad());

    if (bLogWarning && LoadedItem == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
        return nullptr;
    }

    return LoadedItem;
}
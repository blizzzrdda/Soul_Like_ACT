// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulGameInstanceBase.h"
#include "Item/SoulAssetManager.h"
#include "Player/SoulPlayerController.h"


USoulGameInstanceBase::USoulGameInstanceBase()
{
}

void USoulGameInstanceBase::GetAllAccessibleItemID(TArray<FPrimaryAssetId>& OutpId)
{
    //Clear the assets id if we already called this before
    OutpId.Reset();

    for (const auto AssetType : AllItemTypes)
    {
        GetItemIDWithType(AssetType, OutpId);
    }
}

void USoulGameInstanceBase::GetItemIDWithType(const FPrimaryAssetType ItemType, TArray<FPrimaryAssetId>& OutpId)
{
    USoulAssetManager* CurrentAssetManager = USoulAssetManager::Get();

    CurrentAssetManager->GetPrimaryAssetIdList(ItemType, OutpId);
}


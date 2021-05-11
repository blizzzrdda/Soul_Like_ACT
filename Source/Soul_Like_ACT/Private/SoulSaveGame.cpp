// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulSaveGame.h"
#include "Soul_Like_ACT.h"

void USoulSaveGame::ResetSaveGame()
{
    UserId = "Fresh Bird ";
    UserId.AppendInt(FMath::FRandRange(0, 1000));

    InventoryItemData.Init(FSoulSaveItemData(), 40);
    // EquipedItemData.Add(FSoulEquipmentSlot(EGearType::BodyArmor));
    // EquipedItemData.Add(FSoulEquipmentSlot(EGearType::Helmet));
    // EquipedItemData.Add(FSoulEquipmentSlot(EGearType::Weapon));
    // EquipedItemData.Add(FSoulEquipmentSlot(EGearType::Boots));
    // EquipedItemData.Add(FSoulEquipmentSlot(EGearType::Ring));
    // EquipedItemData.Add(FSoulEquipmentSlot(EGearType::Amulet));
    // EquipedItemData.Add(FSoulEquipmentSlot(EGearType::Reliquary));
    // EquipedItemData.Add(FSoulEquipmentSlot(EGearType::Gloves));

    UE_LOG(LogTemp, Warning, TEXT("%s has %d invent slots"), *(FString(__FUNCTION__)), InventoryItemData.Num());
    UE_LOG(LogTemp, Warning, TEXT("%s has %d Equip slots"), *(FString(__FUNCTION__)), EquipedItemData.Num());
    LOG_FUNC_SUCCESS_AUTO();
}

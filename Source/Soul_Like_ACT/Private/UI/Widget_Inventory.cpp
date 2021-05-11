// Fill out your copyright notice in the Description page of Project Settings.

//
// #include "UI/Widget_Inventory.h"
// #include "Components/WrapBox.h"
// #include "Player/InventoryComponent.h"
// #include "Player/EquipmentManager.h"
// #include "Player/Soul_Like_ACTCharacter.h"
//
// void UWidget_Inventory::ConstructInventorySlots()
// {
//     if (!(Class_InventorySlot))
//     {
//         UE_LOG(LogTemp, Error, TEXT("%s ERROR -> %s"), *FString(__FUNCTION__),
//                *FString("Unable to get InventorySlot class"));
//         return;
//     }
//     if (!GetOwningPlayerPawn())
//     {
//         LOG_FUNC_ERROR("No Owning Player");
//         return;
//     }
//     
//     ASoul_Like_ACTCharacter* MyPlayer = Cast<ASoul_Like_ACTCharacter>(GetOwningPlayerPawn());
//     UInventoryComponent* LocalInventComp;
//     UInventoryComponent::GetInventoryManager(this, LocalInventComp);
//     
//     if(LocalInventComp)
//     {
//         for (int32 i = 0; i <= UInventoryComponent::InventorySize; ++i)
//         {
//             auto& SlottedItem = LocalInventComp->InventoryItems[i];
//     
//             UWidget_InventorySlot* LocalSlot = Cast<UWidget_InventorySlot>(
//                 CreateWidget(GetOwningPlayer()
//                              , Class_InventorySlot));
//     
//             LocalSlot->SetupSlot(i, SlottedItem);
//     
//             InventorySlotWrapper->AddChildToWrapBox(LocalSlot);
//             InventorySlots.Add(LocalSlot);
//         }
//     }
//     
//     UEquipmentManager* LocalEquipManager = UEquipmentManager::GetEquipmentManager(this);
//     
//     if(LocalEquipManager)
//     {
//         for(uint8 i = 0; i < MAX_EQUIP_SLOTS; ++i)
//         {
//             auto& SlottedEquip = LocalEquipManager->GetEquipments()[i];
//     
//             UWidget_EquipmentSlot* LocalSlot = Cast<UWidget_EquipmentSlot>(
//                 CreateWidget(GetOwningPlayer()
//                              , Class_EquipmentSlot));
//     
//             LocalSlot->SetupSlot(static_cast<EEquipmentSlot>(i), SlottedEquip);
//     
//             EquipSlotWrapper->AddChildToWrapBox(LocalSlot);
//             EquipmentSlots.Add(LocalSlot);
//             
//         }
//     }
//     
//     BindWiddgets();
//     
//     LOG_FUNC_SUCCESS();
// }
//
// void UWidget_Inventory::BindWiddgets()
// {
//     ASoul_Like_ACTCharacter* MyPlayer = Cast<ASoul_Like_ACTCharacter>(GetOwningPlayerPawn());
//     UInventoryComponent* LocalInventComp;
//     UInventoryComponent::GetInventoryManager(this, LocalInventComp);
//     
//     if (LocalInventComp)
//     {
//         LocalInventComp->OnSlottedItemChanged.AddDynamic(this, &UWidget_Inventory::UpdateInventSlot);
//         //LocalInventComp->OnEquipmentChangedChanged.AddDynamic(this, &UWidget_Inventory::UpdateGearSlot);
//         LOG_FUNC_SUCCESS();
//     }
//     else
//     {
//         UE_LOG(LogTemp, Error, TEXT("%s: Failed to bind on sloted changed"), *this->GetName());
//     }
//
//     //TODO UEquipmentManager Binding
// }
//
// void UWidget_Inventory::UpdateInventSlot(int32 ItemSlot, FInventoryItem Item)
// {
//     auto& LocalWidget = InventorySlots[ItemSlot];
//
//     LocalWidget->SetupSlot(ItemSlot, Item);
// }
//
// void UWidget_Inventory::UpdateGearSlot(EEquipmentSlot EquipSlot, FInventoryItem Item)
// {
//     auto& LocalWidget = EquipmentSlots[static_cast<uint8>(EquipSlot)];
//
//     LocalWidget->SetupSlot(-1, Item);
// }
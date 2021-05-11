// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/SoulPlayerController.h"
#include "AbilitySystemGlobals.h"
#include "Soul_Like_ACT.h"
#include "UINavPCComponent.h"
#include "Player/EquipmentManager.h"
#include "Player/InventoryComponent.h"
#include "Player/PlayerStanceManager.h"

ASoulPlayerController::ASoulPlayerController()
{
    CameraMode = ECameraMode::DetachedCam;

    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");

    EquipmentManager = CreateDefaultSubobject<UEquipmentManager>("EquipmentComponent");

    UINavPCComp = CreateDefaultSubobject<UUINavPCComponent>(TEXT("UINav PC Component"));

    StanceManager = CreateDefaultSubobject<UPlayerStanceManager>(TEXT("Stance Manager"));
}

void ASoulPlayerController::GiveStanceAbilities(APawn* InPawn, UAbilitySystemComponent* ASC)
{
    for(auto& StanceAbilities : StanceManager->ActionSetsPerStance)
    {
        auto PendingAbilities = StanceAbilities.Value.GetAllAbilities();
        for(auto Abi : PendingAbilities)
        {
            FGameplayAbilitySpec AbilitySpec(Abi, 1, INDEX_NONE, this);
            ASC->GiveAbility(AbilitySpec);
        }
    }
}

void ASoulPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    auto ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InPawn);
    check(ASC);

    ASC->InitAbilityActorInfo(InPawn, InPawn);
    
    GiveStanceAbilities(InPawn, ASC);
}

void ASoulPlayerController::SwitchCameraMode(ECameraMode ToCameraMode)
{
    if(CameraMode == ToCameraMode) return;

    
    ASoulPlayerCharacter* PlayerChar = GetPawn() ? Cast<ASoulPlayerCharacter>(GetPawn()) : nullptr;

    if(!PlayerChar)
    {
        LOG_FUNC_FAILURE("Cannot find the possessed character");
        return;
    }
        
    switch (ToCameraMode)
    {
    case ECameraMode::BackFront:
        if(!PlayerChar->Soul_CameraActor->GetChildActor())
        {
            LOG_FUNC_ERROR("Camera Actor not spawned yet");
            break;
        }
        SetViewTargetWithBlend(PlayerChar->Soul_CameraActor->GetChildActor(), 0.3f, EViewTargetBlendFunction::VTBlend_EaseInOut, 1.5f);
        break;
    case ECameraMode::TopDown:
        if(!PlayerChar->TPS_CameraActor->GetChildActor())
        {
            LOG_FUNC_ERROR("Camera Actor not spawned yet");
            break;
        }
        SetViewTargetWithBlend(PlayerChar->TPS_CameraActor->GetChildActor(), 0.3f, EViewTargetBlendFunction::VTBlend_EaseInOut, 1.5f);
        break;
    case ECameraMode::DetachedCam:
        //TODO
        break;
    default:
        //TODO
        break;
    }
    
    CameraMode = ToCameraMode;
}

ASoulPlayerController* ASoulPlayerController::GetSoulPlayerController(UObject* WorldContextObj)
{

    if(WorldContextObj)
    {
        APlayerController* PlayerController = WorldContextObj->GetWorld()->GetFirstPlayerController();
        if(PlayerController)
        {
            return Cast<ASoulPlayerController>(PlayerController);
        }
    }

    return nullptr;
}

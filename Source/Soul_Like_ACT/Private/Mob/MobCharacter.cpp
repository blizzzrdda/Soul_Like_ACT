// Fill out your copyright notice in the Description page of Project Settings.

#include "Mob/MobCharacter.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "Mob/Mob_TargetingComponent.h"
#include "Mob/MobActionManager.h"
#include "Mob/MobController.h"
#include "Item/WeaponActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMobCharacter::AMobCharacter()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    TargetingComponent = CreateDefaultSubobject<UMob_TargetingComponent>(TEXT("TargetingComponent"));

    // ActionManager = CreateDefaultSubobject<UMobActionManager>(TEXT("ActionManager"));

    Faction = EActorFaction::Enemy;

    const TSubclassOf<class UUserWidget> IconWidget = ConstructorHelpers::FClassFinder<UUserWidget>(
        TEXT("/Game/UMG/Mob/w_mob_LockIcon")).Class;
    TargetIcon->SetWidgetClass(IconWidget);
    TargetIcon->SetVisibility(false);
}

void AMobCharacter::ForceOverrideFacingDirection(float Alpha)
{
    AAIController* MobController = Cast<AAIController>(GetController());
    UBlackboardComponent* BB = MobController->GetBlackboardComponent();
    UObject* Target = BB->GetValueAsObject("Target");
    
    if(Target)
    {
        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Cast<AActor>(Target)->GetActorLocation());

        //trim-off roll and pitch axises
        LookAtRotation.Roll = LookAtRotation.Pitch = 0.f;
            
        const FRotator TargetRotation = UKismetMathLibrary::RLerp(GetActorRotation(), LookAtRotation, Alpha, true);

        SetActorRotation(TargetRotation);
    }
}

void AMobCharacter::HandleOnDead(const FHitResult& HitInfo,
    const FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
    //Remove Collision
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

    AController* LocalController = GetController();

    if (LocalController)
        LocalController->UnPossess();

    GetTargetingComponent()->DisableTargeting();

    Faction = EActorFaction::Untargetable;

    StopAnimMontage(GetMesh()->GetAnimInstance()->GetCurrentActiveMontage());
    
    Super::HandleOnDead(HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

AMobController* AMobCharacter::GetMobController() const
{
    if(GetController())
        return Cast<class AMobController>(GetController());
    return nullptr;
}

UMobRageManager* AMobCharacter::GetRageManager() const
{
    if(GetController())
        return  GetMobController()->GetRageManager();
    return nullptr;
}

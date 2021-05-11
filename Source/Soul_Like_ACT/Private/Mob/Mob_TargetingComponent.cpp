// Fill out your copyright notice in the Description page of Project Settings.

#include "Mob/Mob_TargetingComponent.h"
#include "Mob/MobCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UMob_TargetingComponent::UMob_TargetingComponent()
{
    PrimaryComponentTick.bCanEverTick = 1;

    OwnerRef = Cast<class AMobCharacter>(GetOwner());
}


void UMob_TargetingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
    //Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsFacingTarget /* && !OwnerRef->GetIsStun()*/)
    {
        const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
            GetOwner()->GetActorLocation(), TargetPawn->GetActorLocation());

        //TODO: Set rotate speed
        const FRotator Interped_LookAtRotation = FMath::RInterpConstantTo(GetOwner()->GetActorRotation(), LookAtRotation,
                                                                          DeltaTime, InterpSpeed);

        GetOwner()->SetActorRotation(Interped_LookAtRotation);
    }
}

void UMob_TargetingComponent::EnableTargeting(AActor* TargetActor)
{
    TargetPawn  =TargetActor;
    if (TargetPawn)
    {
        bIsFacingTarget = true;
        OwnerRef->GetCharacterMovement()->bOrientRotationToMovement = false;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UMob_TargetingComponent::FacingTarget_Init failed"));
        DisableTargeting();   
    }
}

void UMob_TargetingComponent::DisableTargeting()
{
    bIsFacingTarget = false;
    TargetPawn = nullptr;
    OwnerRef->GetCharacterMovement()->bOrientRotationToMovement = true;
}
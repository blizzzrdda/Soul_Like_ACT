// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/AbilityTasks/AT_PlayMontageAndWaitForEvent.h"
#include "Animation/AnimInstance.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Soul_Like_ACT.h"
#include "GameFramework/Character.h"
#include "Abilities/SoulAbilitySystemComponent.h"

UAT_PlayMontageAndWaitForEvent::UAT_PlayMontageAndWaitForEvent(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    Rate = 1.f;
    bStopWhenAbilityEnds = true;
}

void UAT_PlayMontageAndWaitForEvent::Activate()
{
    if (Ability == nullptr)
    {
        return;
    }

    bool bPlayedMontage = false;
    USoulAbilitySystemComponent* ASC = GetTargetASC();

    if (ASC)
    {
        const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
        UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
        if (AnimInstance != nullptr)
        {
            // Bind to event callback
            EventHandle = ASC->AddGameplayEventTagContainerDelegate(
                EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(
                    this, &UAT_PlayMontageAndWaitForEvent::OnGameplayEvent));

            if (ASC->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay,
                                                        Rate, StartSection) > 0.f)
            {
                // Playing a montage could potentially fire off a callback into game code which could kill this ability! Early out if we are  pending kill.
                if (ShouldBroadcastAbilityTaskDelegates() == false)
                {
                    return;
                }

                CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(
                    this, &UAT_PlayMontageAndWaitForEvent::OnAbilityCancelled);

                BlendingOutDelegate.BindUObject(
                    this, &UAT_PlayMontageAndWaitForEvent::OnMontageBlendingOut);
                AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

                MontageEndedDelegate.BindUObject(this, &UAT_PlayMontageAndWaitForEvent::OnMontageEnded);
                AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

                ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
                if (Character && (Character->GetLocalRole() == ROLE_Authority ||
                    (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() ==
                        EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
                {
                    Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
                }

                bPlayedMontage = true;
            }
        }
        else
        {
            ABILITY_LOG(Warning, TEXT("URPGAbilityTask_PlayMontageAndWaitForEvent call to PlayMontage failed!"));
        }
    }
    else
    {
        ABILITY_LOG(
            Warning, TEXT("URPGAbilityTask_PlayMontageAndWaitForEvent called on invalid AbilitySystemComponent"));
    }

    if (!bPlayedMontage)
    {
        ABILITY_LOG(
            Warning,
            TEXT(
                "URPGAbilityTask_PlayMontageAndWaitForEvent called in Ability %s failed to play montage %s; Task Instance Name %s."
            ), *Ability->GetName(), *GetNameSafe(MontageToPlay), *InstanceName.ToString());
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }

    SetWaitingOnAvatar();
}

void UAT_PlayMontageAndWaitForEvent::ExternalCancel()
{
    check(AbilitySystemComponent);

    OnAbilityCancelled();

    Super::ExternalCancel();
}

FString UAT_PlayMontageAndWaitForEvent::GetDebugString() const
{
    UAnimMontage* PlayingMontage = nullptr;
    if (Ability)
    {
        const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
        UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

        if (AnimInstance != nullptr)
        {
            PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay)
                                 ? MontageToPlay
                                 : AnimInstance->GetCurrentActiveMontage();
        }
    }

    return FString::Printf(
        TEXT("PlayMontageAndWaitForEvent. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(MontageToPlay),
        *GetNameSafe(PlayingMontage));
}

void UAT_PlayMontageAndWaitForEvent::OnDestroy(bool AbilityEnded)
{
    // Note: Clearing montage end delegate isn't necessary since its not a multicast and will be cleared when the next montage plays.
    // (If we are destroyed, it will detect this and not do anything)

    // This delegate, however, should be cleared as it is a multicast
    if (Ability)
    {
        Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
        if (AbilityEnded && bStopWhenAbilityEnds)
        {
            StopPlayingMontage();
        }
    }

    USoulAbilitySystemComponent* RPGAbilitySystemComponent = GetTargetASC();
    if (RPGAbilitySystemComponent)
    {
        RPGAbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandle);
    }

    Super::OnDestroy(AbilityEnded);
}

UAT_PlayMontageAndWaitForEvent* UAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
    UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay,
    FGameplayTagContainer EventTags, float Rate /*= 1.f*/, FName StartSection /*= NAME_None*/,
    bool bStopWhenAbilityEnds /*= true*/, float AnimRootMotionTranslationScale /*= 1.f*/)
{
    UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

    UAT_PlayMontageAndWaitForEvent* MyObj = NewAbilityTask<UAT_PlayMontageAndWaitForEvent>(
        OwningAbility, TaskInstanceName);
    MyObj->MontageToPlay = MontageToPlay;
    MyObj->EventTags = EventTags;
    MyObj->Rate = Rate;
    MyObj->StartSection = StartSection;
    MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
    MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;

    return MyObj;
}

bool UAT_PlayMontageAndWaitForEvent::StopPlayingMontage()
{
    const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
    if (!ActorInfo)
    {
        return false;
    }

    UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
    if (AnimInstance == nullptr)
    {
        return false;
    }

    // Check if the montage is still playing
    // The ability would have been interrupted, in which case we should automatically stop the montage
    if (AbilitySystemComponent && Ability)
    {
        if (AbilitySystemComponent->GetAnimatingAbility() == Ability
            && AbilitySystemComponent->GetCurrentMontage() == MontageToPlay)
        {
            // Unbind delegates so they don't get called as well
            FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
            if (MontageInstance)
            {
                MontageInstance->OnMontageBlendingOutStarted.Unbind();
                MontageInstance->OnMontageEnded.Unbind();
            }

            AbilitySystemComponent->CurrentMontageStop();
            return true;
        }
    }

    return false;
}

USoulAbilitySystemComponent* UAT_PlayMontageAndWaitForEvent::GetTargetASC()
{
    return Cast<USoulAbilitySystemComponent>(AbilitySystemComponent);
}

void UAT_PlayMontageAndWaitForEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
    if (Ability && Ability->GetCurrentMontage() == MontageToPlay)
    {
        if (Montage == MontageToPlay)
        {
            AbilitySystemComponent->ClearAnimatingAbility(Ability);

            // Reset AnimRootMotionTranslationScale
            ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
            if (Character && (Character->GetLocalRole() == ROLE_Authority ||
                (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() ==
                    EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
            {
                Character->SetAnimRootMotionTranslationScale(1.f);
            }
        }
    }

    if (bInterrupted)
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }
    else
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnBlendOut.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }
}

void UAT_PlayMontageAndWaitForEvent::OnAbilityCancelled()
{
    // TODO: Merge this fix back to engine, it was calling the wrong callback

    if (StopPlayingMontage())
    {
        // Let the BP handle the interrupt as well
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }
}

void UAT_PlayMontageAndWaitForEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (!bInterrupted)
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnCompleted.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }

    EndTask();
}

void UAT_PlayMontageAndWaitForEvent::OnGameplayEvent(FGameplayTag EventTag,
                                                                  const FGameplayEventData* Payload)
{
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        FGameplayEventData TempData = *Payload;
        TempData.EventTag = EventTag;

        EventReceived.Broadcast(EventTag, TempData);
    }
}

void UAT_PlayMontageAndWaitForEvent::PlayNewMontage(UAnimMontage* NewMontage, FName SectionName)
{
    if(NewMontage)
    {
        // UnbindAllDelegates();

        MontageToPlay = NewMontage;
        StartSection = NewMontage->IsValidSectionName(SectionName) ? SectionName : NAME_None;

        ReadyForActivation();
    }
    else
    {
        LOG_FUNC_SUCCESS(GetDebugString());
        LOG_FUNC_ERROR("NewMontage or SectionName is invalid");
    }
}

void UAT_PlayMontageAndWaitForEvent::PlayNextSection()
{
    USoulAbilitySystemComponent* ASC = GetTargetASC();

    if (ASC)
    {
        const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
        UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

        if(ASC->GetCurrentMontage() && ASC->GetCurrentMontage() != MontageToPlay)
        {
            LOG_FUNC_FAILURE("A different montage is playing: " + ASC->GetCurrentMontage()->GetName());
            return;
        }
        const auto CurrSectionName = AnimInstance->Montage_GetCurrentSection(MontageToPlay);
        const auto CurrSectionIndex = MontageToPlay->GetSectionIndex(CurrSectionName);
        const auto NextSectionIndex = AnimInstance->Montage_GetNextSectionID(MontageToPlay, CurrSectionIndex);
        const auto NextSectionName = MontageToPlay->GetSectionName(NextSectionIndex);

        UnbindAllDelegates();

        StartSection = NextSectionName;

        Activate();
    }
    else
    {
        LOG_FUNC_ERROR("No ASC found");
    }
}
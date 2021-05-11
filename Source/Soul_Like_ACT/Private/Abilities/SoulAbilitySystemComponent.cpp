// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/SoulAbilitySystemComponent.h"
#include "SoulCharacterBase.h"
#include "AbilitySystemGlobals.h"


void USoulAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer,
                                                             TArray<USoulGameplayAbility*>& ActiveAbilities) const
{
    TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
    GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

    // Iterate the list of all ability specs
    for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
    {
        // Iterate all instances on this ability spec
        TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

        for (UGameplayAbility* ActiveAbility : AbilityInstances)
        {
            ActiveAbilities.Add(Cast<USoulGameplayAbility>(ActiveAbility));
        }
    }
}

int32 USoulAbilitySystemComponent::GetDefaultAbilityLevel() const
{
    ASoulCharacterBase* OwningCharacter = Cast<ASoulCharacterBase>(GetOwnerActor());

    if (OwningCharacter)
    {
        //return OwningCharacter->GetCharacterLevel();
        return 1;
    }
    return 1;
}

USoulAbilitySystemComponent* USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(
    const AActor* Actor, bool LookForComponent)
{
    return Cast<USoulAbilitySystemComponent>(
        UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}

FActiveGameplayEffectHandle USoulAbilitySystemComponent::ApplyEffectToSelf(
    const AActor* Actor, const TSubclassOf<UGameplayEffect> GameplayEffect, const int32 AbilityLevel = 1/*=1*/)
{
    return ApplyEffectToTarget(Actor, Actor, GameplayEffect, AbilityLevel);
}

FActiveGameplayEffectHandle USoulAbilitySystemComponent::ApplyEffectToTarget(
    const AActor* FromActor, const AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffect,
    const int32 AbilityLevel)
{
    USoulAbilitySystemComponent* TempComp = GetAbilitySystemComponentFromActor(TargetActor);
    if (TempComp)
    {
        FGameplayEffectContextHandle EffectContext = TempComp->MakeEffectContext();

        EffectContext.AddSourceObject(FromActor);

        const FGameplayEffectSpecHandle NewHandle = TempComp->MakeOutgoingSpec(GameplayEffect, AbilityLevel, EffectContext);

        if (NewHandle.IsValid())
        {
            return (TempComp->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), TempComp));
        }
    }

    return FActiveGameplayEffectHandle();
}

bool USoulAbilitySystemComponent::TryActivateAbilityByClassWithDelegate(
    FGameplayAbilitySpecHandle& OutAbilitySpecHandle, TSubclassOf<UGameplayAbility> InAbilityToActivate,
    FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate)
{
    bool bSuccess = false;

    const UGameplayAbility* const InAbilityCDO = InAbilityToActivate.GetDefaultObject();

    for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
    {
        if (Spec.Ability == InAbilityCDO)
        {
            OutAbilitySpecHandle = Spec.Handle;
            bSuccess |= TryActivateAbilityWithDelegate(Spec.Handle, true, OnGameplayAbilityEndedDelegate);
            break;
        }
    }

    return bSuccess;
}

bool USoulAbilitySystemComponent::TryActivateAbilityWithDelegate(FGameplayAbilitySpecHandle AbilityToActivate,
                                                                 bool bAllowRemoteActivation,
                                                                 FOnGameplayAbilityEnded::FDelegate*
                                                                 OnGameplayAbilityEndedDelegate /*= nullptr*/)
{
    FGameplayTagContainer FailureTags;
    FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(AbilityToActivate);
    if (!Spec)
    {
        ABILITY_LOG(Warning, TEXT("TryActivateAbility called with invalid Handle"));
        return false;
    }

    UGameplayAbility* Ability = Spec->Ability;

    if (!Ability)
    {
        ABILITY_LOG(Warning, TEXT("TryActivateAbility called with invalid Ability"));
        return false;
    }

    const FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();

    // make sure the ActorInfo and then Actor on that FGameplayAbilityActorInfo are valid, if not bail out.
    if (ActorInfo == nullptr || !ActorInfo->OwnerActor.IsValid() || !ActorInfo->AvatarActor.IsValid())
    {
        return false;
    }

    const ENetRole NetMode = ActorInfo->AvatarActor->GetLocalRole();

    // This should only come from button presses/local instigation (AI, etc).
    if (NetMode == ROLE_SimulatedProxy) return false;

    const bool bIsLocal = AbilityActorInfo->IsLocallyControlled();

    // Check to see if this a local only or server only ability, if so either remotely execute or fail
    if (!bIsLocal && (Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalOnly || Ability->
        GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted))
    {
        if (bAllowRemoteActivation)
        {
            ClientTryActivateAbility(AbilityToActivate);
            return true;
        }

        ABILITY_LOG(Log, TEXT("Can't activate LocalOnly or LocalPredicted ability %s when not local."),
                    *Ability->GetName());
        return false;
    }

    if (NetMode != ROLE_Authority && (Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::ServerOnly
        || Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::ServerInitiated))
    {
        if (bAllowRemoteActivation)
        {
            if (Ability->CanActivateAbility(AbilityToActivate, ActorInfo, nullptr, nullptr, &FailureTags))
            {
                // No prediction key, server will assign a server-generated key
                CallServerTryActivateAbility(AbilityToActivate, Spec->InputPressed, FPredictionKey());
                return true;
            }
            else
            {
                NotifyAbilityFailed(AbilityToActivate, Ability, FailureTags);
                return false;
            }
        }

        ABILITY_LOG(Log, TEXT("Can't activate ServerOnly or ServerInitiated ability %s when not the server."),
                    *Ability->GetName());
        return false;
    }

    return InternalTryActivateAbility(AbilityToActivate, FPredictionKey(), nullptr, OnGameplayAbilityEndedDelegate);
}

void USoulAbilitySystemComponent::AddAttributeValue(const FGameplayAttribute Attribute, float AdditiveValue,
                                                    bool bClampNegative/*= false*/)
{
    const float CurrentValue = GetNumericAttributeBase(Attribute);
    float NewValue = CurrentValue + AdditiveValue;

    if(bClampNegative)
        NewValue = FMath::Clamp(NewValue, 0.f, MAX_flt);
    
    SetNumericAttributeBase(Attribute, NewValue);
}

bool USoulAbilitySystemComponent::IsAbilityGiven(TSubclassOf<UGameplayAbility> Ability)
{
    for(auto& Spec : ActivatableAbilities.Items)
    {
        if(Spec.Ability == Ability.GetDefaultObject())
        {
            //GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red, Spec.Ability->GetName() + " " + Ability.GetDefaultObject()->GetName());
            return true;
        }
    }

    return false;
}
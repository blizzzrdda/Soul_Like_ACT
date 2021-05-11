#include "BPFL/BPFL_AbilitySystem.h"
#include "AbilitySystemComponent.h"

UBPFL_AbilitySystem::UBPFL_AbilitySystem()
{
}

void UBPFL_AbilitySystem::CreateEventData(const AActor* Target, const AActor* Source, const FHitResult& InpHitResult,
                                          const FGameplayTag EventTag, const float EventMagnitude, FGameplayEventData& OutpEventData)
{
    FGameplayEffectContextHandle TempContextHandle(new FGameplayEffectContext());
    TempContextHandle.AddHitResult(InpHitResult);

    FGameplayEventData TempEventData;
    TempEventData.Instigator = Source;
    TempEventData.Target = Target;
    TempEventData.EventMagnitude = EventMagnitude;
    TempEventData.EventTag = EventTag;
    TempEventData.ContextHandle = TempContextHandle;

    OutpEventData = TempEventData;
}

void UBPFL_AbilitySystem::CreateEventDataWithoutHitResult(const AActor* Target, const AActor* Source,
    const FGameplayTag EventTag, const float EventMagnitude, FGameplayEventData& OutpEventData)
{
    FGameplayEventData TempEventData;
    TempEventData.Instigator = Source;
    TempEventData.Target = Target;
    TempEventData.EventMagnitude = EventMagnitude;
    TempEventData.EventTag = EventTag;
    OutpEventData = TempEventData;
}

bool UBPFL_AbilitySystem::OverrideActorGameplayTag(UAbilitySystemComponent* AbilitySysComp, const FGameplayTag& InTag,
    bool bAdd)
{
    if (AbilitySysComp)
    {
        AbilitySysComp->SetTagMapCount(InTag, bAdd);
        return true;
    }

    return false;
}

bool UBPFL_AbilitySystem::DoesActorHasTag(UAbilitySystemComponent* AbilitySysComp, FGameplayTag InTag)
{
    if (AbilitySysComp)
        return AbilitySysComp->HasMatchingGameplayTag(InTag);

    return false;
}

bool UBPFL_AbilitySystem::DoesActorHasAnyTags(UAbilitySystemComponent* AbilitySysComp,
    FGameplayTagContainer InTagContainer)
{
    if (AbilitySysComp)
        return AbilitySysComp->HasAnyMatchingGameplayTags(InTagContainer);

    return false;
}
UGameplayEffectUIData* UBPFL_AbilitySystem::GetActiveGameplayEffectUIData(FActiveGameplayEffectHandle Handle)
{
    UAbilitySystemComponent* ASC = Handle.GetOwningAbilitySystemComponent();
    const FActiveGameplayEffect* ActiveGE = ASC->GetActiveGameplayEffect(Handle);
    if (ActiveGE)
    {
        return ActiveGE->Spec.Def->UIData;
    }

    return nullptr;
}
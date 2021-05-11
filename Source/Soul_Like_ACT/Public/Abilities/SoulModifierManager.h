// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulGameplayAbility.h"
#include "Components/ActorComponent.h"
#include "Item/PA_Item.h"
#include "SoulModifierManager.generated.h"

struct FGameplayAbilitySpecHandle;
struct FGameplayAbilitySpec;
class UGameplayAbility;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API USoulModifierManager : public UActorComponent
{
    GENERATED_BODY()

public:
    USoulModifierManager();
    
protected:
    virtual void BeginPlay() override;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModifiersUpdated, USoulModifierManager*, ModManager);

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TArray<FGameplayAbilitySpecHandle> GrantedActiveAbilities;
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TArray<FGameplayAbilitySpecHandle> GrantedModifiers;
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TArray<FActiveGameplayEffectHandle> GrantedItemEffects;
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TArray<FGameplayAbilitySpecHandle> GrantedInitAbilities;

    UFUNCTION()
    void UpdateAggregators(USoulAbilitySystemComponent* ASC);
    UFUNCTION()
    void UpdateActiveAbilities();
    UFUNCTION()
    void UpdateModifiers();
    UFUNCTION()
    void ApplyItemStats(UPA_Item_Abstract* Item_PA, bool bAdded);
    UFUNCTION()
    void InitStat(USoulAbilitySystemComponent* ASC);
    UFUNCTION()
    void UpdateDefaultAbilities(USoulAbilitySystemComponent* ASC);

public:
    //---------------------------INIT ONLY------------------------------------------
    UPROPERTY(BlueprintReadOnly)
    bool bAbilitiesInitialized;

    // Effects that setup the level 1 character attributes
    UPROPERTY(EditDefaultsOnly, Category = InitialAbilities)
    TSubclassOf<UGameplayEffect> InitStatEffect;
    // Abilities like "Hit Reaction" etc.
    UPROPERTY(EditDefaultsOnly, Category = InitialAbilities)
    UDA_DefaultAbilties* InitAbilities;
    // Attribute equations
    UPROPERTY(EditDefaultsOnly, Category = InitialAbilities)
    UDA_DefaultEffects* AttributeAggregators;
    UFUNCTION()
    void SetupDefaultAbilities();
    //---------------------------UPDATE ABILITIES------------------------------------------

    UFUNCTION(BlueprintCallable, Category = Abilities)
    void UpdateAbilitiesAndModifiers();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = Modifiers)
    void GetActiveModifiers(TArray<UModifierAbility*>& Abilities) const;

    UPROPERTY(BlueprintAssignable, Category = Modifiers)
    FOnModifiersUpdated OnModifiersUpdated;
    
    friend class ASoulCharacterBase;
    friend class UEquipmentManager;
};

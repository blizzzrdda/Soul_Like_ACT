// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Widget_BuffPanel.generated.h"

UCLASS(Abstract)
class SOUL_LIKE_ACT_API UWidget_BuffSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BuffStacks;
	UPROPERTY(meta = (BindWidget))
	UImage* BuffIcon;

	UPROPERTY()
	float Duration;
	UPROPERTY()
	float RemainingDuration;
	UPROPERTY()
	FTimerHandle CoolDownTickingHandle;
	
public:
	UFUNCTION(BlueprintCallable)
    void SetBuffSlot(UAbilitySystemComponent* ASC, const FActiveGameplayEffectHandle& ActiveEffectHandle);

	UFUNCTION()
	void UpdateCoolDown();
};

UCLASS(Abstract)
class SOUL_LIKE_ACT_API UWidget_BuffPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TMap<TSubclassOf<UGameplayEffect>, UWidget_BuffSlot*> BuffEffectSpecs;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidget_BuffSlot> BuffSlotClass;
	
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* BuffBox;
	
public:
	UFUNCTION(BlueprintCallable)
	void BindOnEffectChanged(UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable)
	void OnEffectApplied(UAbilitySystemComponent* ASC, const struct FGameplayEffectSpec& EffectSpec,
                                    struct FActiveGameplayEffectHandle EffectHandle);
	UFUNCTION(BlueprintCallable)
    void BP_OnGameplayEffectRemoved(const struct FActiveGameplayEffect& EffectRemovalInfo);
};


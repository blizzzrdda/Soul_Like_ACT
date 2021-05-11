// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemComponent.h"
#include "Widget_AttributeSlot.generated.h"

class UTextBlock;
struct FOnAttributeChangeData;

/**
 * 
 */
UCLASS(Abstract)
class SOUL_LIKE_ACT_API UWidget_AttributeSlot : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* AttributeType;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* AttributeValue;

	UPROPERTY()
	FGameplayAttribute _Attribute;

	UFUNCTION(BlueprintCallable)
	void Setup(FGameplayAttribute Attribute, UAbilitySystemComponent* ASC);

	void UpdateAttributeValue(const FOnAttributeChangeData& AttributeChangeData) const;
};

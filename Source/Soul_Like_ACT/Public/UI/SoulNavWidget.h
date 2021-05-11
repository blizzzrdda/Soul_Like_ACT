// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UINavWidget.h"
#include "SoulNavWidget.generated.h"

UCLASS()
class SOUL_LIKE_ACT_API USoulNavWidget : public UUINavWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = Default)
	bool bRemoveWhenReturn = true;
	
	UFUNCTION(BlueprintCallable, Category = UINavWidget, meta = (AdvancedDisplay=2))
	UWidget* GoToOnViewportWidget(UUINavWidget* NewWidget, bool bRemoveParent,
		bool bDestroyParent = false);

	virtual void ReturnToParent(bool bRemoveAllParents, int ZOrder) override;
};
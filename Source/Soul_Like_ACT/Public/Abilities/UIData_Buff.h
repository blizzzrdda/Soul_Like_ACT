// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectUIData.h"
#include "UIData_Buff.generated.h"

UCLASS()
class SOUL_LIKE_ACT_API UUIData_Buff : public UGameplayEffectUIData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UIData)
	FName BuffDisplayName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UIData)
	class UTexture* BuffIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UIData)
	bool bBenignBuff = true;
};

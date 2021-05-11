// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DEPRECATED_ANS_WeaponSweeping.generated.h"

UCLASS(Deprecated)
class SOUL_LIKE_ACT_API UDEPRECATED_ANS_WeaponSweeping : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Damage)
	bool DamageMulti = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Damage)
	bool PostureDamageMulti = 1.f;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

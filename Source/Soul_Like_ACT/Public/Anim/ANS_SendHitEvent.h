// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_SendHitEvent.generated.h"

UCLASS(Abstract, NotBlueprintable)
class SOUL_LIKE_ACT_API UANS_SendHitEvent : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnimNotify)
	bool bIgnoreSelf = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnimNotify)
	bool bDrawDebugTrace;
	UPROPERTY()
	bool _DrawDebug = false;

	//X
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnimNotify)
	float Width = 200.f;
	//Y
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnimNotify)
	float Length = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnimNotify)
	float Offset_Forward = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnimNotify)
	float Multi_Damage = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnimNotify)
	float Multi_PostureDamage = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnimNotify)
	float Multi_Crumble = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnimNotify)
	FName Profile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnimNotify)
	FGameplayTag EventTag;

	// The actors have been hit recently
	UPROPERTY()
	TArray<AActor*> ExclusiveActors;
	
	UFUNCTION()
    virtual void DrawTrace(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                            float FrameDeltaTime) {}

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
							float FrameDeltaTime) override
	{
	}


	const FName Profile_Pawn = FName("Pawn");
};

UCLASS()
class UANS_SendHitEvent_Box : public UANS_SendHitEvent
{
	GENERATED_BODY()

protected:
	FVector CalculateStartPosition(USkeletalMeshComponent* MeshComp) const;
	FVector CalculateEndPosition(USkeletalMeshComponent* MeshComp) const;
	FVector CalculateHalfSize(USkeletalMeshComponent* MeshComp) const;
	FRotator CalculateOrientation(USkeletalMeshComponent* MeshComp) const;

public:
	UANS_SendHitEvent_Box()
	{
		EventTag = FGameplayTag::RequestGameplayTag(FName("Event.Montage.Shared.WeaponHit"), true);
	}
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
							float FrameDeltaTime) override;
};

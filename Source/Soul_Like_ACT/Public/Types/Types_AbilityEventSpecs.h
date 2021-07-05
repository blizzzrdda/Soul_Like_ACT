#pragma once
#include "Types_AbilityEventSpecs.generated.h"

//TODO  
USTRUCT(BlueprintType)
struct FAbilityEvent_HitDirection
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector StartPositionOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector EndPositionOffset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Width;
};


USTRUCT(BlueprintType)
struct FAbilityEvent_PushingForceSpecs
{
	GENERATED_BODY()

	FAbilityEvent_PushingForceSpecs()
		: bEnabled(false), Distance(50.f),
		  Duration(0.3f), YawDeviation(0.f)
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bEnabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Distance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Duration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float YawDeviation;
};


USTRUCT(BlueprintType)
struct FAbilityEvent_DamageSpecs
{
	FAbilityEvent_DamageSpecs()
		: bEnabled(false), Multi_Health(1.f),
		  Multi_Posture(1.f), Multi_Crumble(1.f)
	{
	}

	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bEnabled;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Multi_Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Multi_Posture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Multi_Crumble;
};


/**
 * @param bDownWard: Represents the vertical direction of the force.
 *		If it is "true", Height and Duration will be ignored
 * @param Duration: when <= 0.f, it's instant without interpolation.
 */
USTRUCT(BlueprintType)
struct FAbilityEvent_VerticalForceSpecs
{
	FAbilityEvent_VerticalForceSpecs()
		: bEnabled(false), bDownWard(false),
		  Height(100.f), Duration(1.f)
	{
	}

	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bEnabled;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDownWard;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Height;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Duration;
};

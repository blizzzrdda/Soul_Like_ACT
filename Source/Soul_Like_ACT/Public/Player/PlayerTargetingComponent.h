// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerTargetingComponent.generated.h"

USTRUCT()
struct FFacingOffsetContext
{
	FFacingOffsetContext()
		:OffsetSpeed(90.f),
		bUseLockedTarget(true)
	{}
	FFacingOffsetContext(bool bUseTarget, float ToYaw)
		: OffsetSpeed(90.f),
		  bUseLockedTarget(bUseTarget),
		  ToYaw(ToYaw)
	{
	}

	FFacingOffsetContext(float OffsetSpeed, bool bUseTarget, float ToYaw)
		: OffsetSpeed(OffsetSpeed),
		  bUseLockedTarget(bUseTarget),
		  ToYaw(ToYaw)
	{
	}

	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float OffsetSpeed;
	UPROPERTY(EditAnywhere)
	bool bUseLockedTarget;
	UPROPERTY(EditAnywhere)
	float ToYaw;
};

UENUM(BlueprintType)
enum class ETargetFindingDirection : uint8
{
	Centre,
	Left,
	Right,
};

UENUM(BlueprintType)
enum class EFacingOffsetMode : uint8
{
	Disabled,
	Interpolation,
	Const
};

struct FRotationCache
{
	bool bOwnerControllerRotationYaw = false
		 , bOwnerOrientRotToMovement = true
		 , bOwnerControllerDesiredRot = false;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOUL_LIKE_ACT_API UPlayerTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	FTimerHandle TargetBlockingHandler;

	bool bLockCamera;

	UPROPERTY()
	EFacingOffsetMode FacingOffsetMode;
	UPROPERTY()
	FFacingOffsetContext FacingOffsetContext;

	FRotationCache RotationCache;

	UPROPERTY()
	TArray<AActor*> PotentialTargetActors;

	UPROPERTY()
	class ACharacter* PlayerRef;

public:
	UPlayerTargetingComponent();

	UPROPERTY()
	class AActor* LockedTarget;

	UPROPERTY()
	bool isTargetingEnabled;

protected:
	virtual void BeginPlay() override;

	//Detection Stages--------------------------------------------------------------------------------------------------
	void FindTarget(ETargetFindingDirection Direction = ETargetFindingDirection::Centre);

	void GetPotentialTargetsInScreen(TArray<AActor*>& OutPotentialTargets);
	void RuleOutBlockedTargets(TArray<AActor*> LocalPotentialTargets, TArray<AActor*>& OutPotentialTargets);

	void FindClosestTargetInScreen(TArray<AActor*>& LocalPotentialTargets, AActor*& ClosestTarget);
	void Find_InDirection(TArray<AActor*>& LocalPotentialTargets, AActor*& ClosestTarget,
						  ETargetFindingDirection Direction);
	//------------------------------------------------------------------------------------------------------------------

	void EnableLockingTarget();
	void DisableLockingTarget();

	void CacheRotationSetting();
	void ResetRotationSetting() const;

	void SetRotationMode_FaceTarget();

	bool IsTraceBlocked(AActor* SelectedTarget, TArray<AActor*> IgnoredActors, const ECollisionChannel TraceChannel);
	FVector GetLineTraceStartLocation();

	void Timer_CheckBlockingAndDistance();
	bool UpdateRotation_Default(float DeltaTime);
	bool UpdateRotation_Interp_And_Const(float DeltaTime);

	void Tick_UpdateRotation(float DeltaTime);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ToggleCameraLock(bool _bLockCamera);

	UFUNCTION(BlueprintCallable)
	void Toggle_InDirection(ETargetFindingDirection Direction)
	{
		FindTarget(Direction);
	}

	bool GetIsTargetingEnabled() const { return isTargetingEnabled; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = TargetLocking)
	void GetLockedTarget(bool& isEnabled, AActor*& OutLockedTarget) const
	{
		isEnabled = isTargetingEnabled;
		OutLockedTarget = LockedTarget;
	}

	UFUNCTION(BlueprintCallable, Category = TargetLocking)
	void SetFacingOffset(EFacingOffsetMode OffsetMode, float OffsetSpeed = 90.f, bool bUseTarget = true, float ToYaw = 0.f);
	UFUNCTION(BlueprintCallable, Category = TargetLocking)
	void DisableFacingOffset()
	{
		FacingOffsetMode = EFacingOffsetMode::Disabled;
	}
	
	friend class ASoulPlayerCharacter;
};

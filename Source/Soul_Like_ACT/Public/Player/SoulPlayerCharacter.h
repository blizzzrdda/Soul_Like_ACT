// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SoulCharacterBase.h"
#include "Item/WeaponActor.h"
#include "SoulPlayerCharacter.generated.h"

class ASoulPlayerController;

UCLASS(config=Game)
class ASoulPlayerCharacter : public ASoulCharacterBase
{
protected:
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* Soul_CameraBoom;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Camera)
	UChildActorComponent* Soul_CameraActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* TPS_CameraBoom;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Camera)
	UChildActorComponent* TPS_CameraActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* TargetLockArrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UPlayerTargetingComponent* TargetingComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = AI)
	class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSource;

public:
	static const float BattleMovementScale;
	static const float TravelMovementScale;

	ASoulPlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(meta = (DeprecatedNode))
	bool _bFreeCamera = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	float ForwardAxisValue, RightAxisValue;
	float LeanAmount_Char, LeanSpeed_Char;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float LeanAmount_Anim;

protected:
	//Tick------------------------------
	UFUNCTION()
	void MoveForward(float Value);
	UFUNCTION()
	void MoveRight(float Value);

	void MakeMove();

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void ZoomCamera(float Rate);

	UFUNCTION(BlueprintCallable)
	void CalculateLeanValue(float TurnValue);
	//----------------------------------

	UFUNCTION(BlueprintCallable)
	bool IsMovementInputPressed() const
	{
		return !(FMath::IsNearlyZero(ForwardAxisValue) && FMath::IsNearlyZero(RightAxisValue));
	}

	UFUNCTION(BlueprintImplementableEvent)
	void DegreeToMovementMultiplier(const float& Degree, float& Multiplier);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * The function is called when some attack movements have root motion enabled and direction offsets.
	 * It overrides the direction with controller's rotation or by calling lock-target component;
	 */
	virtual void ForceOverrideFacingDirection(float Alpha) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	void PredictMovement(FVector& DirectionVec, float& Degree) const;

	//------------CAMERA---------------
	UFUNCTION(BlueprintCallable, Category = Camera)
	void GetActiveCameraAndSpringArm(ACameraActor*& ActiveCam, USpringArmComponent*& ActiveSpringArm,
									 bool& isValid) const;
	//-----------CAMERA------------------

	UFUNCTION(BlueprintCallable)
	void ResetRotation();

	UFUNCTION(BlueprintCallable, meta = (DeprecatedFunction))
	AWeaponActor* EquipGear(TSubclassOf<AWeaponActor> WeaponClassRef, bool bShowTracelines);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASoulPlayerController* GetSoulController() const;

	virtual AWeaponActor* GetWeaponActor() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(WorldContext="WorldContextObj"))
	static ASoulPlayerCharacter* GetSoulPlayerChar(UObject* WorldContextObj);
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContext"), Category = TargetLockingComponent)
	static class UPlayerTargetingComponent* GetPlayerTargetLockingComponent(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta=( DeprecatedNode), Category = Camera)
	void SetCameraMode(bool bFreeCamera) { _bFreeCamera = bFreeCamera; }

	friend class ASoulPlayerController;
};

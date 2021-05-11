// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "InputBufferPlayerController.h"
#include "SoulPlayerCharacter.h"
#include "UINavPCReceiver.h"
#include "SoulPlayerController.generated.h"

UENUM()
enum class ECameraMode : uint8
{
	BackFront,
	TopDown,
	DetachedCam,
};

UCLASS()
class SOUL_LIKE_ACT_API ASoulPlayerController : public AInputBufferPlayerController, public IUINavPCReceiver
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UEquipmentManager* EquipmentManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UUINavPCComponent* UINavPCComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UPlayerStanceManager* StanceManager;

	UPROPERTY()
	ASoulPlayerCharacter* PlayerPawn;

	UPROPERTY()
	ECameraMode CameraMode;

public:
	ASoulPlayerController();
	void GiveStanceAbilities(APawn* InPawn, UAbilitySystemComponent* ASC);

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable)
	ECameraMode GetCamMode() const { return CameraMode; }

	UFUNCTION(BlueprintCallable)
	class UEquipmentManager* GetEquipmentManager() const { return EquipmentManager; }

	UFUNCTION(BlueprintCallable)
	void SwitchCameraMode(ECameraMode ToCameraMode);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(WorldContext="WorldContextObj"))
	static ASoulPlayerController* GetSoulPlayerController(UObject* WorldContextObj);
};

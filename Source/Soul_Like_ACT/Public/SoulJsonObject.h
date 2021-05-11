// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BlueprintJsonLibrary.h"
#include "SoulJsonObject.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SOUL_LIKE_ACT_API USoulJsonObjectWrapper : public UObject
{
	GENERATED_BODY()

protected:
	static const FString ActionDamageMultiField;
	static const FString ActionPostureDamageMultiField;
	static const FString ActionCrumbleMultiField;

	static const FString PushingDirectionField;
	static const FString PushingDistanceField;
	static const FString PushingDurationField;

	static const FString LiftingHeightField;
	static const FString LiftingDurationField;

public:
	USoulJsonObjectWrapper()
		: Super()
	{
		JsonObject = UBlueprintJsonLibrary::JsonMake();
	}

	UPROPERTY(BlueprintReadWrite, Category = Json)
	FBlueprintJsonObject JsonObject;

	UFUNCTION(BlueprintPure, Category = Json)
	static USoulJsonObjectWrapper* MakeSoulJsonObject()
	{
		return NewObject<USoulJsonObjectWrapper>();
	}

	UFUNCTION(BlueprintCallable, category = Json)
	static void SoulSetJsonField(USoulJsonObjectWrapper* JsonObjectWrapper, const FString& FieldName,
								 const FBlueprintJsonValue& JsonValue)
	{
		if (JsonObjectWrapper->JsonObject.Object.IsValid() && JsonValue.Value.IsValid())
		{
			JsonObjectWrapper->JsonObject.Object->SetField(FieldName, JsonValue.Value);
		}
	}

	//--------------------------------------Anim Combo----------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = Abilities)
	USoulJsonObjectWrapper* SetJumpSection(bool isSameMontage,
										   FString JumpTargetName);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Abilities)
	void JsonGetJumpSection(bool& isSameMontage,
							FString& JumpTargetName, bool& bSuccessful) const;

	//-------------------------------------Push Force-----------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = Abilities)
	USoulJsonObjectWrapper* SetPushingForce(FVector Direction, float Distance, float Duration);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Abilities)
	void JsonGetPushingForce(FVector& Direction, float& Distance, float& Duration, bool& bSuccessful) const;

	//-------------------------------------Shoryuken------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = Abilities)
	USoulJsonObjectWrapper* SetLiftingForce(float Height, float Duration);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Abilities)
	void JsonGetLiftingForce(float& Height, float& Duration, bool& bSuccessful) const;

	//-------------------------------------Damage Multi---------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = Damage)
	USoulJsonObjectWrapper* SetActionDamageMulties(float InDamageMulti,
												   float InPostureDamageMulti, float InCrumbleMulti);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Abilities)
	void JsonGetActionDamageMulties(float& OutDamageMulti,
									float& OutPostureDamageMulti, float& OutCrumbleMulti, bool& bSuccessful) const;
};

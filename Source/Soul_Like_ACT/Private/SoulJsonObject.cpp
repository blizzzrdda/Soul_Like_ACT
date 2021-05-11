// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulJsonObject.h"

const FString USoulJsonObjectWrapper::ActionDamageMultiField = FString("ActionDamageMulti");
const FString USoulJsonObjectWrapper::ActionPostureDamageMultiField = FString("ActionPostureDamageMulti");
const FString USoulJsonObjectWrapper::ActionCrumbleMultiField = FString("ActionCrumbleMulti");

const FString USoulJsonObjectWrapper::PushingDirectionField = FString("PushingDirection");
const FString USoulJsonObjectWrapper::PushingDistanceField = FString("PushingDistance");
const FString USoulJsonObjectWrapper::PushingDurationField = FString("PushingDuration");

const FString USoulJsonObjectWrapper::LiftingHeightField = FString("LiftingHeight");
const FString USoulJsonObjectWrapper::LiftingDurationField = FString("LiftingDuration");

USoulJsonObjectWrapper* USoulJsonObjectWrapper::SetJumpSection(bool isSameMontage, FString JumpTargetName)
{
	if (JsonObject.Object.IsValid())
	{
		JsonObject.Object->SetBoolField("isSameMontage", isSameMontage);

		JsonObject.Object->SetStringField("JumpTargetName", JumpTargetName);
	}

	return this;
}

void USoulJsonObjectWrapper::JsonGetJumpSection(bool& isSameMontage,
												FString& JumpTargetName, bool& bSuccessful) const
{
	if (JsonObject.Object.IsValid() && (JsonObject.Object->HasField("isSameMontage")
		&& JsonObject.Object->HasField("JumpTargetName")))
	{
		bSuccessful = true;

		isSameMontage = JsonObject.Object->GetBoolField("isSameMontage");
		JumpTargetName = JsonObject.Object->GetStringField("JumpTargetName");

		return;
	}

	bSuccessful = false;
}

USoulJsonObjectWrapper* USoulJsonObjectWrapper::SetPushingForce(FVector Direction, float Distance, float Duration)
{
	if (JsonObject.Object.IsValid())
	{
		JsonObject.Object->SetStringField(PushingDirectionField, Direction.ToString());

		JsonObject.Object->SetNumberField(PushingDistanceField, Distance);

		JsonObject.Object->SetNumberField(PushingDurationField, Duration);
	}

	return this;
}

void USoulJsonObjectWrapper::JsonGetPushingForce(FVector& Direction, float& Distance, float& Duration,
												 bool& bSuccessful) const
{
	if (JsonObject.Object.IsValid() && (JsonObject.Object->HasField(PushingDirectionField)
		&& JsonObject.Object->HasField(PushingDistanceField) && JsonObject.Object->HasField(PushingDurationField)))
	{
		const auto DirString = JsonObject.Object->GetStringField(PushingDirectionField);
		bSuccessful = Direction.InitFromString(DirString);
		
		Distance = JsonObject.Object->GetNumberField(PushingDistanceField);
		
		Duration = JsonObject.Object->GetNumberField(PushingDurationField);

		return;
	}

	bSuccessful = false;
}

USoulJsonObjectWrapper* USoulJsonObjectWrapper::SetLiftingForce(float Height, float Duration)
{
	if (JsonObject.Object.IsValid())
	{
		JsonObject.Object->SetNumberField(LiftingHeightField, Height);

		JsonObject.Object->SetNumberField(LiftingDurationField, Duration);
	}

	return this;
}

void USoulJsonObjectWrapper::JsonGetLiftingForce(float& Height, float& Duration, bool& bSuccessful) const
{
	if (JsonObject.Object.IsValid() && (JsonObject.Object->HasField(LiftingHeightField)
		&& JsonObject.Object->HasField(LiftingDurationField)))
	{
		Height = JsonObject.Object->GetNumberField(LiftingHeightField);
		Duration = JsonObject.Object->GetNumberField(LiftingDurationField);

		bSuccessful = true;

		return;
	}

	bSuccessful = false;
}

USoulJsonObjectWrapper* USoulJsonObjectWrapper::SetActionDamageMulties(float InDamageMulti, float InPostureDamageMulti,
																	   float InCrumbleMulti)
{
	if (JsonObject.Object.IsValid())
	{
		JsonObject.Object->SetNumberField(ActionDamageMultiField, InDamageMulti);

		JsonObject.Object->SetNumberField(ActionPostureDamageMultiField, InPostureDamageMulti);

		JsonObject.Object->SetNumberField(ActionCrumbleMultiField, InCrumbleMulti);
	}

	return this;
}

void USoulJsonObjectWrapper::JsonGetActionDamageMulties(
	float& OutDamageMulti, float& OutPostureDamageMulti, float& OutCrumbleMulti, bool& bSuccessful) const
{
	if (JsonObject.Object.IsValid() &&
		JsonObject.Object->HasField(ActionDamageMultiField) &&
		JsonObject.Object->HasField(ActionPostureDamageMultiField) &&
		JsonObject.Object->HasField(ActionCrumbleMultiField))
	{
		bSuccessful = true;
		OutDamageMulti = JsonObject.Object->GetNumberField(ActionDamageMultiField);
		OutPostureDamageMulti = JsonObject.Object->GetNumberField(ActionPostureDamageMultiField);
		OutCrumbleMulti = JsonObject.Object->GetNumberField(ActionCrumbleMultiField);
		return;
	}

	bSuccessful = false;
}

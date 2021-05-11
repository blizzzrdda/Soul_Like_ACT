// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define GET_ENUM_STRING(etype, evalue) ((FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true) != nullptr) \
	? FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true)->GetEnumName((int32)evalue) \
	: FString("Invalid - are you sure enum uses UENUM() macro?"))

#define LOG_FUNC_SUCCESS_AUTO() UE_LOG(LogTemp, Display, TEXT("%s successful"), *FString(__FUNCTION__))
#define LOG_FUNC_FAILURE(Message) UE_LOG(LogTemp, Warning, TEXT("%s failed -> %s "), *FString(__FUNCTION__), *FString(Message))
#define LOG_FUNC_ERROR(Message) UE_LOG(LogTemp, Error, TEXT("%s ERROR -> %s "), *FString(__FUNCTION__), *FString(Message))
#define LOG_FUNC_SUCCESS(Message) UE_LOG(LogTemp, Display, TEXT("%s Successful -> %s "), *FString(__FUNCTION__), *FString(Message))

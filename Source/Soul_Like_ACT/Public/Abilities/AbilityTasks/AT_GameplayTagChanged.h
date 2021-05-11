// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "AT_GameplayTagChanged.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayTagAddedRemoved, FGameplayTag, Tag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameplayTagChanged, FGameplayTag, Tag, int32, Num);


/**
* Blueprint node to automatically register a listener for FGameplayTags added and removed.
* Useful to use in Blueprint/UMG.
*/
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class SOUL_LIKE_ACT_API UAT_GameplayTagAddedRemoved : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnGameplayTagAddedRemoved OnTagAdded;

	UPROPERTY(BlueprintAssignable)
	FOnGameplayTagAddedRemoved OnTagRemoved;

	// Listens for FGameplayTags added and removed.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UAT_GameplayTagAddedRemoved* ListenForGameplayTagAddedOrRemoved(
		UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer InTags);

	// You must call this function manually when you want the AsyncTask to end.
	// For UMG Widgets, you would call it in the Widget's Destruct event.
	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	UAbilitySystemComponent* ASC;

	FGameplayTagContainer Tags;

	virtual void TagChanged(const FGameplayTag Tag, int32 NewCount);
};

UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class SOUL_LIKE_ACT_API UAT_GameplayTagChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnGameplayTagChanged OnTagChanged;

	// Listens for FGameplayTags added and removed.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
    static UAT_GameplayTagChanged* ListenForGameplayTagChanged(
        UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer InTags);

	// You must call this function manually when you want the AsyncTask to end.
	// For UMG Widgets, you would call it in the Widget's Destruct event.
	UFUNCTION(BlueprintCallable)
    void EndTask();

	protected:
	UPROPERTY()
	UAbilitySystemComponent* ASC;

	FGameplayTagContainer Tags;
	
	virtual void TagChanged(const FGameplayTag Tag, int32 NewCount);
};

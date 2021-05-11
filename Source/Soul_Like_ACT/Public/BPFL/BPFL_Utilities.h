#pragma once
#include "Item/PA_Item.h"
#include "BPFL_Utilities.generated.h"

/** Static class with useful gameplay utility functions that can be called from both Blueprint and C++ */
UCLASS()
class SOUL_LIKE_ACT_API UBPFL_Utilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// UFUNCTION(BlueprintCallable)
	// static void GetRenderedActors(TArray<AActor*>& CurrentlyRenderedActors, float MinRecentTime = 0.01f);
	//
	// UFUNCTION(BlueprintCallable)
	// static void GetNotRenderedActors(TArray<AActor*>& CurrentlyNotRenderedActors, float MinRecentTime = 0.01f); 

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext="WorldContext"), Category = Inventory)
	static class UEquipmentManager* GetEquipmentManager(UObject* WorldContext);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext="WorldContext"), Category = Inventory)
    static class UInventoryComponent* GetInventoryManager(UObject* WorldContext);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext="WorldContext"), Category = ModifierManager)
	static class USoulModifierManager* GetModifierManager(UObject* WorldContext);

	UFUNCTION(BlueprintCallable, meta = (WorldContext="WorldContext"), Category = Inventory)
	static bool MergeModifierArray(UPARAM(ref) TArray<FSoulModifier>& A);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = AttributeSet)
	static FString GetFormattedAttributeValue(FGameplayAttribute Attribute, float Value);
};

template<typename KeyType>
TMap<KeyType, int32>& MergeIntMap(TMap<KeyType, int32>& A, const TMap<KeyType, int32>& B)
{
	for(auto pair : B)
	{
		A.FindOrAdd(pair.Key) += pair.Value;
	}

	return A;
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SoulGameplayAbility.h"
#include "Components/ActorComponent.h"
#include "Types/PlayerMoveSet.h"
#include "PlayerStanceManager.generated.h"

USTRUCT(BlueprintType)
struct SOUL_LIKE_ACT_API FPlayerActionSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UActiveAbility> Step;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UActiveAbility> Roll;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UActiveAbility> Parry;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UActiveAbility> Attack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UActiveAbility> ParryAttack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UActiveAbility> DodgeAttack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UActiveAbility> ParryDodge;

	TArray<TSubclassOf<UActiveAbility>> GetAllAbilities() const
	{
		TArray<TSubclassOf<UActiveAbility>> Result;

		// for (TFieldIterator<FProperty> prop(StaticStruct()); prop; ++prop){
		//
		// 	FClassProperty* class_prop = CastField<FClassProperty>(*prop);
		// 	if (class_prop && class_prop->MetaClass) {
		// 		LOG_FUNC_SUCCESS(class_prop->MetaClass->GetName());
		// 		Result.Add(Cast<UActiveAbility>(class_prop->MetaClass)->StaticClass());
		// 	}
		// }

		if (Step) Result.Add(Step);
		if (Roll) Result.Add(Roll);
		if (Parry)Result.Add(Parry);
		if (Attack) Result.Add(Attack);
		if (ParryAttack) Result.Add(ParryAttack);
		if (DodgeAttack) Result.Add(DodgeAttack);
		if (ParryDodge) Result.Add(ParryDodge);

		return Result;
	}
};

UCLASS(NotBlueprintable)
class SOUL_LIKE_ACT_API UPlayerStanceManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerStanceManager();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stance)
	TMap<EStanceType, FPlayerActionSet> ActionSetsPerStance;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Stance)
	EStanceType ActiveStance;

	UFUNCTION(BlueprintCallable)
	void ActivateStance(EStanceType NewStance)
	{
		ActiveStance = NewStance;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FPlayerActionSet GetActiveStanceSet() const;
};

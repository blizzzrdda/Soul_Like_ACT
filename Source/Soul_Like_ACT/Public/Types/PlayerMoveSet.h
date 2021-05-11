#pragma once
#include "CoreMinimal.h"
#include "PlayerMoveSet.generated.h"

class UAnimMontage;

UENUM(BlueprintType)
enum class ESheathType : uint8
{
	Sheathed,
	Unsheathed
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Light,
	Heavy,
	Channeling,
};

UENUM(BlueprintType)
enum class EStanceType : uint8
{
	NonWeapon = 0,
	OnShoulder = 1,
	Middle = 2,
	Low = 3
};

USTRUCT(BlueprintType)
struct FAttackMontageSet
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Animation)
	UAnimMontage* LightAttack;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Animation)
	UAnimMontage* HeavyAttack;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Animation)
	UAnimMontage* ChannelingAttack;

	bool CanProcLightAttack() const
	{
		return LightAttack != nullptr;
	}

	bool CanProcHeavyAttack() const
	{
		return HeavyAttack != nullptr && LightAttack != nullptr;
	}

	bool CanProcChannelingAttack() const
	{
		return ChannelingAttack != nullptr;
	}

	bool IsValid() const
	{
		return CanProcChannelingAttack() || CanProcHeavyAttack() || CanProcLightAttack();
	}

	/**
	 * @brief Get the anim montage based on attack type
	 * @return 
	 */
	UAnimMontage* GetMontage_UnSafe(EAttackType AttackType) const
	{
		switch (AttackType)
		{
			case EAttackType::Light: return LightAttack;
			case EAttackType::Heavy: return HeavyAttack;
			case EAttackType::Channeling: return ChannelingAttack;
			default: return nullptr;
		}
	}

	/**
	 * @brief	Get the montage based on AttackType.
	 *			If there is no anim montage found, return the *PRIOR* montage, by following the rule: Channeling > Heavy > Light
	 * @return	Returns false if no montage is valid
	 */
	bool GetMontage_Safe(EAttackType AttackType, UAnimMontage*& Montage) const
	{
		Montage = GetMontage_UnSafe(AttackType);

		if (!Montage)
			Montage = ChannelingAttack;
		if (!Montage)
			Montage = HeavyAttack;
		if (!Montage)
			Montage = LightAttack;

		return Montage != nullptr;
	}
};

USTRUCT(BlueprintType)
struct FAttackCombo
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
	TArray<FAttackMontageSet> ComboSets;

public:
	const TArray<FAttackMontageSet>& GetComboSets() const
	{
		return ComboSets;
	}

	int32 GetComboLength() const
	{
		return ComboSets.Num();
	}

	bool IsValid() const
	{
		if (ComboSets.Num() == 0) return false;

		for (auto& MontageSet : ComboSets)
		{
			if (!MontageSet.IsValid())
				return false;
		}

		return true;
	}
};

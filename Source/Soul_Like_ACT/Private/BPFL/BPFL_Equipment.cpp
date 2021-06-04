// Fill out your copyright notice in the Description page of Project Settings.


#include "BPFL/BPFL_Equipment.h"

FGameplayTag UBPFL_Equipment::GetWeaponAttackSoundTag(ESoundSelector_MeleeAttack SoundSelector)
{
	switch (SoundSelector) {
		case ESoundSelector_MeleeAttack::Attacking:
			return FGameplayTag::RequestGameplayTag("Event.Montage.Shared.Sound.Attacking");
		case ESoundSelector_MeleeAttack::HitTarget:
			return FGameplayTag::RequestGameplayTag("Event.Montage.Shared.Sound.WeaponHit");
		case ESoundSelector_MeleeAttack::Post:
			return FGameplayTag::RequestGameplayTag("Event.Montage.Shared.Sound.Post");
		default:
			return FGameplayTag::RequestGameplayTag("Event.Montage.Shared.Sound.Attacking");
	}
}

bool UBPFL_Equipment::StanceToSocketName(EStanceType Stance, FName& OutSocketName)
{
	switch (Stance)
	{
		case EStanceType::OnShoulder:
			OutSocketName = FName("Soc_Top");
			break;
		case EStanceType::Middle:
			OutSocketName = FName("Soc_Top");
			break;
		case EStanceType::Low:
			OutSocketName = FName("Soc_Top");
			break;
		default:
			return false;;
	}

	return true;
}

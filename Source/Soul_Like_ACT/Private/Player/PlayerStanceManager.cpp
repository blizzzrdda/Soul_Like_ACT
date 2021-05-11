// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerStanceManager.h"
#include "Soul_Like_ACT.h"

UPlayerStanceManager::UPlayerStanceManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

FPlayerActionSet UPlayerStanceManager::GetActiveStanceSet() const
{
	if (ActionSetsPerStance.Contains(ActiveStance))
	{
		return ActionSetsPerStance[ActiveStance];
	}

	LOG_FUNC_ERROR("ActionSetsPerStance is not complete. Must fix this");
	return FPlayerActionSet();
}

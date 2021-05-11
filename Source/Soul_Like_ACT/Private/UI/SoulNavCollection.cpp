// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SoulNavCollection.h"
#include "UINavWidget.h"

void USoulNavCollection::OnNavigate_Implementation(int From, int To, int LocalFrom, int LocalTo)
{
	if(Delegate_NavTo.IsBound())
		Delegate_NavTo.Broadcast(From, To, LocalFrom, LocalTo);	
}

void USoulNavCollection::OnSelect_Implementation(int Index, int LocalIndex)
{
	if(Delegate_Select.IsBound())
		Delegate_Select.Broadcast(Index, LocalIndex);
}

void USoulNavCollection::SetCollectionEnabled_Implementation(bool bEnable)
{
	if(bEnable)
	{
		// SetIsEnabled(true);
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		// SetIsEnabled(false);
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void USoulNavCollection::ForceIncreGrids(int Increment)
{
	IncrementGrids(Increment);
}

void UHighlightableCollection::HighlightWithMultiTypes_Implementation(const TArray<FPrimaryAssetType>& Types, bool bReset)
{
	if(bReset)
		Execute_CancelHighlights(this);

	for(const auto Iter : Types)
	{
		Execute_HighlightWithAssetType(this, Iter, false);
	} 
}

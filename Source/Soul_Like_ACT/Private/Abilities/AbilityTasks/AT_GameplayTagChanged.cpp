#include "Abilities/AbilityTasks/AT_GameplayTagChanged.h"


UAT_GameplayTagAddedRemoved* UAT_GameplayTagAddedRemoved::ListenForGameplayTagAddedOrRemoved(
	UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer InTags)
{
	UAT_GameplayTagAddedRemoved* ListenForGameplayTagAddedRemoved = NewObject<UAT_GameplayTagAddedRemoved>();
	ListenForGameplayTagAddedRemoved->ASC = AbilitySystemComponent;
	ListenForGameplayTagAddedRemoved->Tags = InTags;

	if (!IsValid(AbilitySystemComponent) || InTags.Num() < 1)
	{
		ListenForGameplayTagAddedRemoved->EndTask();
		return nullptr;
	}

	TArray<FGameplayTag> TagArray;
	InTags.GetGameplayTagArray(TagArray);

	for (FGameplayTag LocalTag : TagArray)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(LocalTag, EGameplayTagEventType::NewOrRemoved).
								AddUObject(ListenForGameplayTagAddedRemoved, &UAT_GameplayTagAddedRemoved::TagChanged);
	}

	return ListenForGameplayTagAddedRemoved;
}

void UAT_GameplayTagAddedRemoved::EndTask()
{
	if (IsValid(ASC))
	{
		TArray<FGameplayTag> TagArray;
		Tags.GetGameplayTagArray(TagArray);

		for (FGameplayTag LocalTag : TagArray)
		{
			ASC->RegisterGameplayTagEvent(LocalTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		}
	}

	SetReadyToDestroy();
	MarkPendingKill();
}

void UAT_GameplayTagAddedRemoved::TagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		OnTagAdded.Broadcast(Tag);
	}
	else
	{
		OnTagRemoved.Broadcast(Tag);
	}
}

UAT_GameplayTagChanged* UAT_GameplayTagChanged::ListenForGameplayTagChanged(
	UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer InTags)
{
	UAT_GameplayTagChanged* ListenForGameplayTagChanged = NewObject<UAT_GameplayTagChanged>();
	ListenForGameplayTagChanged->ASC = AbilitySystemComponent;
	ListenForGameplayTagChanged->Tags = InTags;

	if (!IsValid(AbilitySystemComponent) || InTags.Num() < 1)
	{
		ListenForGameplayTagChanged->EndTask();
		return nullptr;
	}

	TArray<FGameplayTag> TagArray;
	InTags.GetGameplayTagArray(TagArray);

	for (const FGameplayTag LocalTag : TagArray)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(LocalTag, EGameplayTagEventType::AnyCountChange).
                                AddUObject(ListenForGameplayTagChanged, &UAT_GameplayTagChanged::TagChanged);
	}
	return ListenForGameplayTagChanged;
}

void UAT_GameplayTagChanged::EndTask()
{
	if (IsValid(ASC))
	{
		TArray<FGameplayTag> TagArray;
		Tags.GetGameplayTagArray(TagArray);

		for (const FGameplayTag LocalTag : TagArray)
		{
			ASC->RegisterGameplayTagEvent(LocalTag, EGameplayTagEventType::AnyCountChange).RemoveAll(this);
		}
	}

	SetReadyToDestroy();
	MarkPendingKill();
}

void UAT_GameplayTagChanged::TagChanged(const FGameplayTag Tag, int32 NewCount)
{
	OnTagChanged.Broadcast(Tag, NewCount);
}
